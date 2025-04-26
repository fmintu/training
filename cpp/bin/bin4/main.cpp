
#include <iostream>
#include <thread>

#include "../../common/util.h"
#include "../../lib/math/math.h"
#include "../../lib/shape/shape.h"
#include "../../lib/thread/thread.h"

int main() {
  std::cout << "This is bin4!" << std::endl;
  std::cout << square(5) << std::endl;
  print_hello();

  Notifier n;

  std::thread t1([&] { n.wait(); });
  std::thread t2([&] {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    n.signal();
  });

  t1.join();
  t2.join();
  return 0;
}