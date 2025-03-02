#ifndef STATUS_H
#define STATUS_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

extern std::mutex mtx;

extern std::mutex cv_mtx;
extern std::condition_variable cv;
extern bool ready;
extern int counter;

extern int mem;
extern int cpu;

long long getTotalCPUTime();
long long getProcessCPUTime();
void cpuMonitor(int interval);

int getMemoryUsage();
void memoryMonitor(int interval);

void cpuMonitor(int interval);

void trackingMonitor(int interval);
void notifyMonitor(int interval);

#endif