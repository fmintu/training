
#include "status.h"

int main() {
  std::thread memThread(memoryMonitor, 2);
  std::thread cpuThread(cpuMonitor, 2);
  std::thread trackingThread(trackingMonitor, 2);
  std::thread notifyThread(notifyMonitor, 2);

  notifyThread.join();
  cpuThread.join();
  trackingThread.join();
  memThread.join();

  return 0;
}
