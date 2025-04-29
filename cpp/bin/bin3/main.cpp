
#include <arpa/inet.h>  // sockaddr_in, inet_pton
#include <unistd.h>     // close

#include <cstring>
#include <iostream>
#include <string>

#include "../../common/util.h"
#include "../../lib/math/math.h"
#include "../../lib/shape/shape.h"
#include "json.hpp"

using json = nlohmann::json;

void send_json(int sock, const json& j) {
  std::string message = j.dump();
  send(sock, message.c_str(), message.size(), 0);

  char buffer[4096];
  memset(buffer, 0, sizeof(buffer));
  int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);

  if (bytes_received > 0) {
    std::cout << "Server response:\n" << buffer << "\n";
  } else {
    std::cout << "No response or connection closed.\n";
  }
}

int main() {
  const std::string SERVER_IP = "127.0.0.1";
  const int SERVER_PORT = 12345;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "Failed to create socket.\n";
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, SERVER_IP.c_str(), &server_addr.sin_addr);

  if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Connection failed.\n";
    return 1;
  }

  std::cout << "Connected to server!\n";

  while (true) {
    std::string cmd;
    std::cout << "\nEnter command (get/post/quit): ";
    std::getline(std::cin, cmd);

    if (cmd == "quit") break;

    if (cmd == "get") {
      std::string endpoint;
      std::cout << "GET path (e.g., /api/info | /api/status | /api/user): ";
      std::getline(std::cin, endpoint);

      json j;
      j["type"] = "GET";
      j["data"] = endpoint;
      send_json(sock, j);
    } else if (cmd == "post") {
      std::string action;
      std::cout << "POST action (e.g., login, upload): ";
      std::getline(std::cin, action);

      json j;
      j["type"] = "POST";
      j["data"]["action"] = action;

      if (action == "login") {
        std::string username;
        std::cout << "Username: ";
        std::getline(std::cin, username);
        j["data"]["username"] = username;
      } else if (action == "upload") {
        std::string content;
        std::cout << "Upload content: ";
        std::getline(std::cin, content);
        j["data"]["file"] = content;
      }

      send_json(sock, j);
    } else {
      std::cout << "Unknown command.\n";
    }
  }

  close(sock);
  return 0;
}