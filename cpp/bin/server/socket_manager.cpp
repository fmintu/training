#include "socket_manager.h"

SocketManager::SocketManager(int port) : port_(port) {
  server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd_ < 0) throw std::runtime_error("Socket creation failed");
}

SocketManager::~SocketManager() {
  close(server_fd_);
  for (auto& [fd, conn] : connection_table_) {
    if (conn.thread_handle.joinable()) conn.thread_handle.join();
  }
}

void SocketManager::Init() {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port_);

  if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0)
    throw std::runtime_error("Bind failed");
  if (listen(server_fd_, SOMAXCONN) < 0)
    throw std::runtime_error("Listen failed");

  std::cout << "Server listening on port " << port_ << "...\n";
}

std::optional<std::pair<int, sockaddr_in>> SocketManager::AcceptClient() {
  sockaddr_in client_addr{};
  socklen_t len = sizeof(client_addr);
  int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &len);
  if (client_fd < 0) return std::nullopt;
  return std::make_pair(client_fd, client_addr);
}