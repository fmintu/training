// controller.h
#pragma once

#include <thread>
#include <vector>

#include "db_manager.h"
#include "socket_manager.h"

class Controller {
 public:
  Controller(DbManager* dbmgr, SocketManager* sockmgr);
  ~Controller();

  void Init();
  void Wait();

 private:
  DbManager* dbmanager_;
  SocketManager* socket_manager_;
  std::vector<std::thread> workers_;
};