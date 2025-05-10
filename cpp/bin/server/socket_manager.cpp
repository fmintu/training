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
  // while (true) {
  //   sockaddr_in client_addr{};
  //   socklen_t len = sizeof(client_addr);
  //   int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &len);
  //   if (client_fd < 0) continue;
  //   std::thread t(&SocketManager::ClientThread, this, client_fd,
  //   client_addr);
  //   {
  //     std::lock_guard lock(table_mutex_);
  //     connection_table_[client_fd] =
  //         ClientConnection{client_fd, std::move(t), true};
  //   }
  // }
}

// void SocketManager::ClientThread(int client_fd, sockaddr_in addr) {
//   std::cout << "Client connected: " << inet_ntoa(addr.sin_addr) << ":"
//             << ntohs(addr.sin_port) << "\n";
//   char buf[4096];
//   while (true) {
//     std::memset(buf, 0, sizeof(buf));
//     int n = recv(client_fd, buf, sizeof(buf) - 1, 0);
//     if (n <= 0) break;
//     json req = json::parse(buf);
//     json res = handler_->Handle(req);  // message process -> return
//     std::string out = res.dump();
//     send(client_fd, out.c_str(), out.size(), 0);
//   }
//   close(client_fd);
//   std::lock_guard lock(table_mutex_);
//   connection_table_[client_fd].active = false;
// }

std::optional<std::pair<int, sockaddr_in>> SocketManager::AcceptClient() {
  sockaddr_in client_addr{};
  socklen_t len = sizeof(client_addr);
  int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &len);
  if (client_fd < 0) return std::nullopt;
  return std::make_pair(client_fd, client_addr);
}