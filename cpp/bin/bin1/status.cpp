#include "status.h"

std::mutex mtx;
int counter = 0;
int mem = 0;
int cpu = 0;
std::queue<std::pair<int, int>> queue;

std::mutex cv_mtx;
std::condition_variable cv;
bool ready = false;

int send_message(std::string message) {
  int sock = 0;
  struct sockaddr_in serv_addr;

  char buffer[1024] = {0};

  // Creating the socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Socket creation error" << std::endl;
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cout << "Invalid address/ Address not supported" << std::endl;
    return -1;
  }

  // Connect to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Connection Failed" << std::endl;
    return -1;
  }

  // Send message to the server
  send(sock, message.c_str(), message.length(), 0);
  std::cout << "Message sent" << std::endl;

  // Read response from the server
  read(sock, buffer, 1024);
  std::cout << "Message from server: " << buffer << std::endl;

  // Close the socket
  close(sock);

  return 0;
}

long long get_total_cpu_time() {
  std::ifstream procStat("/proc/stat");
  std::string line;
  getline(procStat, line);
  std::istringstream iss(line);
  std::string cpu;
  long long user, nice, system, idle, iowait, irq, softirq, steal;
  iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
      steal;
  return user + nice + system + idle + iowait + irq + softirq + steal;
}

long long get_process_cpu_time() {
  std::ifstream procStat("/proc/self/stat");
  std::string value;
  long long utime, stime, cutime, cstime;

  for (int i = 1; i <= 13; ++i) procStat >> value;
  procStat >> utime >> stime >> cutime >> cstime;
  return utime + stime + cutime + cstime;
}

void cpu_monitor(int interval) {
  const long ticks_per_second = sysconf(_SC_CLK_TCK);
  long long prev_total_time = get_total_cpu_time();
  long long prev_proc_time = get_process_cpu_time();

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(interval));

    mtx.lock();

    long long curr_total_time = get_total_cpu_time();
    long long curr_proc_time = get_process_cpu_time();

    long long total_delta = curr_total_time - prev_total_time;
    long long proc_delta = curr_proc_time - prev_proc_time;

    double cpu_usage = 100.0 * (proc_delta / static_cast<double>(total_delta)) /
                       ticks_per_second;

    std::cout << "CPU Usage: " << cpu_usage << "%" << std::endl;
    cpu = cpu_usage;

    prev_total_time = curr_total_time;
    prev_proc_time = curr_proc_time;

    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }
}
/// MEM
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
  while (true) {
    mtx.lock();
    mem = get_memory_usage();
    // int MEM = getMemoryUsage();
    std::cout << "Memory Usage: " << mem << " KB" << std::endl;

    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }
}

void tracking_monitor(int interval) {
  while (true) {
    if (mtx.try_lock()) {
      counter++;
      std::cout << "Counter: " << counter << std::endl;
      queue.push({cpu, mem});

      std::this_thread::sleep_for(std::chrono::seconds(interval));
      mtx.unlock();

      if (counter % 5 == 0) {  // if
        std::unique_lock<std::mutex> lock(cv_mtx);
        ready = true;
        cv.notify_all();
      }

    } else {
      std::cout << "Thread could not lock the mutex." << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void notify_monitor(int interval) {
  while (true) {
    std::unique_lock<std::mutex> lock(cv_mtx);
    cv.wait(lock, [] { return ready; });

    std::cout << "Notify thread is processing data\n";
    int c_cpu = queue.front().first;
    int c_mem = queue.front().second;
    queue.pop();

    std::string message = std::to_string(c_cpu) + "|" + std::to_string(c_mem);
    send_message(message);

    ready = false;
    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }
}
