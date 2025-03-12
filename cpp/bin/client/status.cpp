#include "status.h"

std::mutex mtx;
int counter = 0;

std::mutex cv_mtx;
std::condition_variable cv;
bool ready = false;

/******************************  SHARED MEMORY *******************************/
/******************************************************************************/
int write(int option, int value) {  // 0 = cpu, 1 = mem
  // Create or open the shared memory object
  mtx.lock();
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    std::cerr << "Error creating shared memory." << std::endl;
    return -1;
  }

  // Set the size of the shared memory object
  if (ftruncate(shm_fd, SHM_SIZE) == -1) {
    std::cerr << "Error setting size of shared memory." << std::endl;
    return -1;
  }

  // Map the shared memory object into memory
  void* addr =
      mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  if (addr == MAP_FAILED) {
    std::cerr << "Error mapping shared memory." << std::endl;
    return -1;
  }

  // Write two integers to shared memory
  int* shared_data = static_cast<int*>(addr);
  shared_data[option] = value;  // 0 = cpu, 1 = mem

  mtx.unlock();
  return 0;
}

int read(int option) {  // 0 = cpu, 1 = mem
  mtx.lock();
  // Open the shared memory object
  int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
  if (shm_fd == -1) {
    std::cerr << "Error opening shared memory." << std::endl;
    return -1;
  }

  // Map the shared memory object into memory
  void* addr = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
  if (addr == MAP_FAILED) {
    std::cerr << "Error mapping shared memory." << std::endl;
    return -1;
  }

  // Read the two integers from shared memory
  int* shared_data = static_cast<int*>(addr);  // Cast to int pointer
  // std::cout << "Integers read from shared memory: " << shared_data[0] << "
  // and "
  //           << shared_data[1] << std::endl;
  mtx.unlock();
  return shared_data[option];
}
/****************************** CPU THREAD ************************************/
/******************************************************************************/
double get_cpu_usage() {
  std::ifstream file("/proc/stat");
  std::string line;
  std::getline(file, line);  // Read the first line (CPU statistics)

  std::istringstream ss(line);
  std::string cpu;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal >> guest >> guest_nice;

  // Calculate total time and idle time
  long total_time =
      user + nice + system + idle + iowait + irq + softirq + steal;
  long idle_time = idle + iowait;

  // Calculate CPU usage
  double cpu_usage = 100.0 * (total_time - idle_time) / total_time;
  return cpu_usage;
}

void cpu_monitor(int interval) {
  try {
    while (true) {
      double cpu_usage = get_cpu_usage();

      write(0, cpu_usage);
      std::cout << "CPU Usage: " << cpu_usage << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
  } catch (const std::exception& e) {
    std::cerr << "Error in cpu_monitor: " << e.what() << std::endl;
    mtx.unlock();
  }
}
/****************************** MEMORY THREAD *******************************/
/******************************************************************************/
int get_memory_usage() {
  std::string line;
  int memory = 0;
  std::ifstream statusFile("/proc/self/status");

  while (getline(statusFile, line)) {
    if (line.substr(0, 6) == "VmSize") {
      std::istringstream iss(line);
      std::string key;
      int mem;
      std::string unit;
      iss >> key >> mem >> unit;
      if (unit == "kB") {
        memory = mem;
      }
      break;
    }
  }
  statusFile.close();
  return memory;
}

void memory_monitor(int interval) {
  try {
    while (true) {
      int memory_usage = get_memory_usage();
      write(1, memory_usage);

      std::cout << "Memory Usage: " << memory_usage << " KB" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
  } catch (const std::exception& e) {
    std::cerr << "Error in memory_monitor: " << e.what() << std::endl;
    mtx.unlock();
  }
}
/****************************** TRACKING THREAD *******************************/
/******************************************************************************/
void tracking_monitor(int interval) {
  try {
    while (true) {
      counter++;
      std::cout << "Counter: " << counter << std::endl;

      if (counter % 5 == 0) {  // if
        std::unique_lock<std::mutex> lock(cv_mtx);
        ready = true;
        cv.notify_all();
      }
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
  } catch (const std::exception& e) {
    std::cerr << "Error in tracking_monitor: " << e.what() << std::endl;
    mtx.unlock();
  }
}

/********************************** NOTIFY THREAD *****************************/
/******************************************************************************/
int send_message(std::string message) {
  int sock = 0;
  struct sockaddr_in serv_addr;

  char buffer[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "--- Socket creation error" << std::endl;
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cout << "--- Invalid address/ Address not supported" << std::endl;
    return -1;
  }

  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "--- Connection Failed" << std::endl;
    return -1;
  }

  send(sock, message.c_str(), message.length(), 0);
  std::cout << "--- Message sent" << std::endl;

  read(sock, buffer, 1024);
  std::cout << "--- Message from server: " << buffer << std::endl;

  close(sock);

  return 0;
}

void notify_monitor(int interval) {
  try {
    while (true) {
      std::unique_lock<std::mutex> lock(cv_mtx);
      cv.wait(lock, [] { return ready; });

      // std::cout << "Notify thread is processing data\n";
      int c_cpu = read(0);
      int c_mem = read(1);

      std::string message =
          "CPU:" + std::to_string(c_cpu) + "|MEM:" + std::to_string(c_mem);

      send_message(message);

      ready = false;
      std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
  } catch (const std::exception& e) {
    std::cerr << "Error in notify_monitor: " << e.what() << std::endl;
    mtx.unlock();
  }
}
