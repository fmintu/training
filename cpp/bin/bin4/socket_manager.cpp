#include "socket_manager.h"

using json = nlohmann::json;
// ################################ INIT ##################################
SocketManager::SocketManager(int port, DbManager* dbmgr)
    : port_(port), dbManager_(dbmgr) {
  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);

  registerHandler();
  if (server_socket_ < 0) {
    throw std::runtime_error("Socket creation failed");
  }
}

SocketManager::~SocketManager() {
  close(server_socket_);
  // join all threads
  for (auto& [fd, conn] : connection_table_) {
    if (conn.thread_handle.joinable()) conn.thread_handle.join();
  }
}

void SocketManager::Init() {
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port_);

  if (bind(server_socket_, (sockaddr*)&addr, sizeof(addr)) < 0)
    throw std::runtime_error("Bind failed");
  if (listen(server_socket_, SOMAXCONN) < 0)
    throw std::runtime_error("Listen failed");

  std::cout << "Server listening on port " << port_ << "...\n";

  while (true) {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(server_socket_, (sockaddr*)&client_addr, &len);
    if (client_fd < 0) {
      std::cerr << "Accept failed.\n";
      continue;
    }
    std::thread t(&SocketManager::client_handler, this, client_fd, client_addr);
    {
      std::lock_guard lock(table_mutex_);
      connection_table_[client_fd] =
          ClientConnection{client_fd, std::move(t), true};
    }
  }
}
// ################################ HANDLER ##################################
void SocketManager::client_handler(int client_socket, sockaddr_in client_addr) {
  std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << ":"
            << ntohs(client_addr.sin_port) << "\n";

  char buffer[4096];
  while (true) {
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
      std::cout << "Client disconnected.\n";
      std::lock_guard lock(table_mutex_);
      connection_table_[client_socket].active = false;
      break;
    }
    std::string req(buffer);
    std::string resp = process_message(req);
    send(client_socket, resp.c_str(), resp.size(), 0);
  }
  close(client_socket);
}

std::string SocketManager::process_message(const std::string& message) {
  try {
    json j = json::parse(message);
    std::string type = j["type"];
    if (type == "GET") return handle_get(j).dump();
    if (type == "POST") return handle_post(j).dump();
    if (type == "JSON") return handle_json(j).dump();

    return json{{"status", "error"}, {"message", "Unknown type: " + type}}
        .dump();
  } catch (const std::exception& e) {
    return json{{"status", "error"},
                {"message", std::string("Invalid JSON: ") + e.what()}}
        .dump();
  }
}

json SocketManager::handle_get(const json& j) {
  std::string path = j.value("data", "");
  auto it = get_handlers_.find(path);
  if (it != get_handlers_.end()) return it->second();
  return json{{"status", "error"}, {"message", "No GET handler for " + path}};
}

json SocketManager::handle_post(const json& j) {
  auto obj = j.value("data", json{});

  std::string action = obj.value("action", "");
  auto it = post_handlers_.find(action);
  if (it != post_handlers_.end()) return it->second(obj);
  return json{{"status", "error"},
              {"message", "No POST handler for " + action}};
}

json SocketManager::handle_json(const json& j) {
  return json{{"status", "ok"},
              {"message", "Generic JSON received"},
              {"echo", j["data"]}};
}

void SocketManager::registerGetHandler(const std::string& path, GetHandler h) {
  get_handlers_.emplace(path, std::move(h));
}

void SocketManager::registerPostHandler(const std::string& action,
                                        PostHandler h) {
  post_handlers_.emplace(action, std::move(h));
}

void SocketManager::registerHandler() {
  // ------------- GET HANDLERS --------------
  registerGetHandler("/api/info", []() {
    return json{{"status", "ok"}, {"message", "This is API info"}};
  });
  registerGetHandler("/api/status", []() {
    return json{{"status", "ok"}, {"message", "This is API status"}};
  });
  registerGetHandler("/api/user", []() {
    return json{{"status", "ok"}, {"message", "This is API user"}};
  });

  // ------------- POST HANDLERS --------------
  registerPostHandler("login", [this](const json& req) {
    std::string user = req.value("username", "unknown");
    std::cout << "write to database" << std::endl;
    dbManager_->logLoginHistory(user);  // database
    dbManager_->showAllLoginHistory();  // database
    return json{{"status", "ok"}, {"message", "User " + user + " logged in"}};
  });

  registerPostHandler("upload", [](const json& req) {
    return json{{"status", "ok"},
                {"message", "Upload received: " + req.dump()}};
  });
}
