// lib/thread/thread.h
#pragma once
#include <condition_variable>
#include <mutex>

class Notifier {
 public:
  void wait();
  void signal();

 private:
  std::mutex mtx;
  std::condition_variable cv;
  bool ready = false;
};