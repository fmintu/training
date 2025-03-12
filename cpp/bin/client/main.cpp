
#include "status.h"

int main() {
  std::thread mem_thread(memory_monitor, 2);
  std::thread cpu_thread(cpu_monitor, 2);
  std::thread tracking_thread(tracking_monitor, 2);
  std::thread notify_thread(notify_monitor, 2);

  cpu_thread.join();
  mem_thread.join();

  tracking_thread.join();
  notify_thread.join();

  try {
    std::thread mem_thread(memory_monitor, 2);
    std::thread cpu_thread(cpu_monitor, 2);
    std::thread tracking_thread(tracking_monitor, 2);
    std::thread notify_thread(notify_monitor, 2);

    cpu_thread.join();
    mem_thread.join();

    tracking_thread.join();
    notify_thread.join();
  } catch (const std::exception& e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    return -1;
  } catch (...) {
    std::cerr << "An unknown error occurred." << std::endl;
    return -1;
  }

  return 0;
}
