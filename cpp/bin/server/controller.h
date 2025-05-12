#pragma once

#include <netinet/in.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "db_manager.h"
#include "request_handler.h"
#include "socket_manager.h"

class Controller {
 public:
  Controller(SocketManager* sockmgr, RequestHandler* handler);
  ~Controller();

  void Init();
  void Wait();

 private:
  void AcceptLoop();
  void HandleClient(int client_fd, sockaddr_in addr);

  DbManager* dbmanager_;
  SocketManager* socket_manager_;
  RequestHandler* handler_;

  std::thread accept_thread_;
  std::vector<std::thread> workers_;
  std::mutex thread_mutex_;
};
