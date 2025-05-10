#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "json.hpp"
#include "request_handler.h"

using json = nlohmann::json;

struct ClientConnection {
  int socket_fd;
  std::thread thread_handle;
  bool active;
};

class SocketManager {
 public:
  SocketManager(int port);

  ~SocketManager();
  void Init();
  std::optional<std::pair<int, sockaddr_in>> AcceptClient();

 private:
  void ClientThread(int client_fd, sockaddr_in addr);
  int port_;
  int server_fd_;

  std::unordered_map<int, ClientConnection> connection_table_;
  std::mutex table_mutex_;
};