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
#include <queue>
#include <sstream>
#include <string>
#include <thread>
extern std::mutex mtx;

extern std::mutex cv_mtx;
extern std::condition_variable cv;
extern bool ready;
extern int counter;

extern std::queue<std::pair<int, int> > queue;
extern int cpu;
extern int mem;

long long get_total_cpu();
long long get_process_cpu();
void cpu_monitor(int interval);

int get_memory_usage();
void memory_monitor(int interval);

void cpu_monitor(int interval);

void tracking_monitor(int interval);
void notify_monitor(int interval);
int send_message(std::string message);
#endif