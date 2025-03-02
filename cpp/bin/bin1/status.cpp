#include "status.h"

std::mutex mtx;
int counter = 0;
int mem = 0;
int cpu = 0;

std::mutex cv_mtx;
std::condition_variable cv;
bool ready = false;

long long getTotalCPUTime() {
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

// Function to get process CPU time
long long getProcessCPUTime() {
  std::ifstream procStat("/proc/self/stat");
  std::string value;
  long long utime, stime, cutime, cstime;

  for (int i = 1; i <= 13; ++i) procStat >> value;  // Skip initial values
  procStat >> utime >> stime >> cutime >> cstime;
  return utime + stime + cutime + cstime;
}

void cpuMonitor(int interval) {
  const long ticks_per_second = sysconf(_SC_CLK_TCK);
  long long prevTotalTime = getTotalCPUTime();
  long long prevProcTime = getProcessCPUTime();

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(interval));

    mtx.lock();

    long long currTotalTime = getTotalCPUTime();
    long long currProcTime = getProcessCPUTime();

    long long totalDelta = currTotalTime - prevTotalTime;
    long long procDelta = currProcTime - prevProcTime;

    double cpuUsage = 100.0 * (procDelta / static_cast<double>(totalDelta)) /
                      ticks_per_second;

    std::cout << "CPU Usage: " << cpuUsage << "%" << std::endl;
    cpu = cpuUsage;
    // Update previous values for next calculation
    prevTotalTime = currTotalTime;
    prevProcTime = currProcTime;

    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }
}
/// MEM
int getMemoryUsage() {
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

void memoryMonitor(int interval) {
  while (true) {
    mtx.lock();
    mem = getMemoryUsage();
    // int MEM = getMemoryUsage();
    std::cout << "Memory Usage: " << mem << " KB" << std::endl;

    mtx.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }
}

void trackingMonitor(int interval) {
  while (true) {
    if (mtx.try_lock()) {
      counter++;
      std::cout << "Counter: " << counter << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(interval));
      mtx.unlock();

      if (mem < 10.0 || cpu < 10.0) {
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

void notifyMonitor(int interval) {
  std::unique_lock<std::mutex> lock(cv_mtx);
  cv.wait(lock, [] { return ready; });  // Chờ cho đến khi ready == true
  std::cout << "Notify thread is processing data\n";
  std::this_thread::sleep_for(std::chrono::seconds(interval));
}
