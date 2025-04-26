// lib/thread/thread.cpp
#include "thread.h"

#include <iostream>
#include <thread>

void Notifier::wait() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [&] { return ready; });
  std::cout << "Received signal!\n";
}

void Notifier::signal() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
  }
  cv.notify_one();
}