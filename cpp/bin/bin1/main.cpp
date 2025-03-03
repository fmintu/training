
#include "status.h"

int main() {
  std::thread mem_thread(memory_monitor, 2);
  std::thread cpu_thread(cpu_monitor, 2);
  std::thread tracking_thread(tracking_monitor, 2);
  std::thread notify_thread(notify_monitor, 2);

  notify_thread.join();
  cpu_thread.join();
  tracking_thread.join();
  mem_thread.join();

  return 0;
}
