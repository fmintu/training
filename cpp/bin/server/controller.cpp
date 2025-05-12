#include "controller.h"

#include <cstring>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

Controller::Controller(SocketManager* sockmgr, RequestHandler* handler)
    : socket_manager_(sockmgr), handler_(handler) {}

Controller::~Controller() {
  if (accept_thread_.joinable()) accept_thread_.join();
  for (auto& t : workers_) {
    if (t.joinable()) t.join();
  }
}

void Controller::Init() {
  socket_manager_->Init();
  accept_thread_ = std::thread(&Controller::AcceptLoop, this);
}

void Controller::Wait() {
  if (accept_thread_.joinable()) accept_thread_.join();
}

void Controller::AcceptLoop() {
  while (true) {
    auto client_opt = socket_manager_->AcceptClient();
    if (!client_opt) continue;

    int client_fd = client_opt->first;
    sockaddr_in addr = client_opt->second;

    std::thread t(&Controller::HandleClient, this, client_fd, addr);

    {
      std::lock_guard<std::mutex> lock(thread_mutex_);
      workers_.emplace_back(std::move(t));
    }
  }
}

void Controller::HandleClient(int client_fd, sockaddr_in addr) {
  std::cout << "Client connected: " << inet_ntoa(addr.sin_addr) << ":"
            << ntohs(addr.sin_port) << "\n";

  char buf[4096];
  while (true) {
    std::memset(buf, 0, sizeof(buf));
    int n = recv(client_fd, buf, sizeof(buf) - 1, 0);
    if (n <= 0) break;

    json req = json::parse(buf, nullptr, false);
    if (req.is_discarded()) continue;

    json res = handler_->Handle(req);
    std::string out = res.dump();
    send(client_fd, out.c_str(), out.size(), 0);
  }

  close(client_fd);
}