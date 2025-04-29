#pragma once
#include <mutex>
#include <thread>
#include <unordered_map>

struct ClientConnection {
  int socket_fd;
  std::thread thread_handle;
  bool active;
};

extern std::unordered_map<int, ClientConnection> connection_table;
extern std::mutex table_mutex;