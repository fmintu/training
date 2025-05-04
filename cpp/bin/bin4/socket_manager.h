#pragma once

#include <arpa/inet.h>  // inet_ntoa, sockaddr_in
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>  // socket, bind, listen, accept, close

#include <cstring>  // memset
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "db_manager.h"
#include "json.hpp"

class SocketManager {
 public:
  explicit SocketManager(int port, DbManager* db);
  ~SocketManager();

  void Init();

 private:
  struct ClientConnection {
    int socket_fd;
    std::thread thread_handle;
    bool active;
  };

  void client_handler(int client_socket, sockaddr_in client_addr);

  std::string process_message(const std::string& message);

  using json = nlohmann::json;
  using GetHandler = std::function<json()>;
  using PostHandler = std::function<json(const json&)>;

  json handle_get(const json& j);
  json handle_post(const json& j);
  json handle_json(const json& j);

  void registerHandler();
  void registerGetHandler(const std::string& path, GetHandler h);
  void registerPostHandler(const std::string& action, PostHandler h);

  std::unordered_map<std::string, GetHandler> get_handlers_;
  std::unordered_map<std::string, PostHandler> post_handlers_;

  int port_;
  int server_socket_;
  std::mutex table_mutex_;
  DbManager* dbManager_;
  std::unordered_map<int, ClientConnection> connection_table_;
};
