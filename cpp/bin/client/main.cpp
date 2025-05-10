#include <arpa/inet.h>  // sockaddr_in, inet_pton
#include <unistd.h>     // close

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "../../common/util.h"
#include "../../lib/math/math.h"
#include "../../lib/shape/shape.h"
#include "json.hpp"

using json = nlohmann::json;

// ================= CONNECT =================
bool connect_to_server(int& sock, const std::string& ip, int port) {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "Failed to create socket.\n";
    return false;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

  if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Connection failed.\n";
    close(sock);
    return false;
  }

  std::cout << "✅ Connected to server!\n";
  return true;
}

// ================= SEND JSON =================
bool send_json(int& sock, const json& j, const std::string& ip, int port) {
  std::string message = j.dump();
  if (send(sock, message.c_str(), message.size(), 0) < 0) {
    std::cerr << "Send failed. Trying to reconnect...\n";
    if (!connect_to_server(sock, ip, port)) return false;
    if (send(sock, message.c_str(), message.size(), 0) < 0) return false;
  }

  char buffer[4096] = {0};
  int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);

  if (bytes_received > 0) {
    std::cout << "Server response:\n" << buffer << "\n";
    return true;
  } else {
    std::cerr << "Connection lost. Attempting to reconnect...\n";
    if (!connect_to_server(sock, ip, port)) return false;
    return false;
  }
}

// ================= MAIN =================
int main() {
  const std::string SERVER_IP = "127.0.0.1";
  const int SERVER_PORT = 123456;
  int sock;

  if (!connect_to_server(sock, SERVER_IP, SERVER_PORT)) return 1;

  while (true) {
    std::string cmd;
    std::cout << "\nEnter command (get/post/quit): ";
    std::getline(std::cin, cmd);

    if (cmd == "quit") break;

    json j;
    if (cmd == "get") {
      std::string endpoint;
      std::cout << "GET path (e.g., /api/info | /api/status | /api/user): ";
      std::getline(std::cin, endpoint);

      j["type"] = "GET";
      j["data"] = endpoint;
    } else if (cmd == "post") {
      std::string action;
      std::cout << "POST action (e.g., login, register): ";
      std::getline(std::cin, action);

      j["type"] = "POST";
      j["data"]["action"] = action;

      if (action == "login" || action == "register") {
        std::string username, password;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        std::cout << "Password: ";
        std::getline(std::cin, password);

        j["data"]["username"] = username;
        j["data"]["password"] = password;
      }
    } else {
      std::cout << " Unknown command.\n";
      continue;
    }

    if (!send_json(sock, j, SERVER_IP, SERVER_PORT)) {
      std::cerr << "Failed to send data after reconnect.\n";
    }
  }

  close(sock);
  return 0;
}