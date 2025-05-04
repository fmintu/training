#include "controller.h"

Controller::Controller(DbManager* dbmgr, SocketManager* sockmgr)
    : dbmanager_(dbmgr), socket_manager_(sockmgr) {}

Controller::~Controller() { Wait(); }

void Controller::Init() {
  workers_.emplace_back([this]() { socket_manager_->Init(); });
}

void Controller::Wait() {
  for (auto& t : workers_) {
    if (t.joinable()) {
      t.join();
    }
  }
}