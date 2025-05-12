#include <arpa/inet.h>  // sockaddr_in, inet_pton
#include <unistd.h>     // close

#include <cstdlib>  // EXIT_SUCCESS, EXIT_FAILURE
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "json.hpp"

using json = nlohmann::json;

// Attempts to (re)connect; returns true if sock is valid.
bool Connect(int& sock, const std::string& ip, int port) {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cerr << "Failed to create socket\n";
    return false;
  }

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

  if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    std::cerr << "Connection to " << ip << ':' << port << " failed\n";
    close(sock);
    return false;
  }

  std::cout << "Connected to server\n";
  return true;
}

bool SendAndReceive(int& sock, const json& j, const std::string& ip, int port) {
  std::string payload = j.dump();
  if (send(sock, payload.c_str(), payload.size(), 0) < 0) {
    std::cerr << "Send failed; reconnecting...\n";
    if (!Connect(sock, ip, port)) return false;
    if (send(sock, payload.c_str(), payload.size(), 0) < 0) {
      std::cerr << "Re-send still failed\n";
      return false;
    }
  }

  char buffer[4096] = {};
  int r = recv(sock, buffer, sizeof(buffer) - 1, 0);
  if (r <= 0) {
    std::cerr << "Receive failed; reconnecting...\n";
    if (!Connect(sock, ip, port)) return false;
    return false;
  }

  std::cout << "Server response:\n" << buffer << "\n";
  return true;
}

int main() {
  const std::string SERVER_IP = "127.0.0.1";
  const int SERVER_PORT = 123456;
  int sock;

  if (!Connect(sock, SERVER_IP, SERVER_PORT)) return EXIT_FAILURE;

  while (true) {
    std::cout << "\nEnter action (create_profile, get_profile, update_profile, "
                 "delete_profile,\n"
                 "             create_account, get_account, update_password, "
                 "delete_account,\n"
                 "             register_account, login, quit): ";
    std::string action;
    std::getline(std::cin, action);
    if (action == "quit") break;

    json j;
    j["action"] = action;

    if (action == "create_profile") {
      std::string first_name, last_name, email;
      std::cout << "First name: ";
      std::getline(std::cin, first_name);
      std::cout << "Last name: ";
      std::getline(std::cin, last_name);
      std::cout << "Email: ";
      std::getline(std::cin, email);
      j["first_name"] = first_name;
      j["last_name"] = last_name;
      j["email"] = email;

    } else if (action == "get_profile" || action == "delete_profile") {
      int id;
      std::cout << "Profile ID: ";
      std::cin >> id;
      std::cin.ignore();
      j["id"] = id;

    } else if (action == "update_profile") {
      int id;
      std::string first_name, last_name, email;
      std::cout << "Profile ID: ";
      std::cin >> id;
      std::cin.ignore();
      std::cout << "New first name: ";
      std::getline(std::cin, first_name);
      std::cout << "New last name: ";
      std::getline(std::cin, last_name);
      std::cout << "New email: ";
      std::getline(std::cin, email);
      j["id"] = id;
      j["first_name"] = first_name;
      j["last_name"] = last_name;
      j["email"] = email;

    } else if (action == "create_account") {
      std::string username, password;
      int profile_id;
      std::cout << "Username: ";
      std::getline(std::cin, username);
      std::cout << "Password: ";
      std::getline(std::cin, password);
      std::cout << "Profile ID: ";
      std::cin >> profile_id;
      std::cin.ignore();
      j["username"] = username;
      j["password"] = password;
      j["profile_id"] = profile_id;

    } else if (action == "get_account" || action == "update_password") {
      std::string username;
      std::cout << "Username: ";
      std::getline(std::cin, username);
      j["username"] = username;
      if (action == "update_password") {
        std::string new_password;
        std::cout << "New password: ";
        std::getline(std::cin, new_password);
        j["new_password"] = new_password;
      }

    } else if (action == "delete_account") {
      int id;
      std::cout << "Account ID: ";
      std::cin >> id;
      std::cin.ignore();
      j["id"] = id;

    } else if (action == "register_account") {
      std::string username, password, first_name, last_name, email;
      std::cout << "Username: ";
      std::getline(std::cin, username);
      std::cout << "Password: ";
      std::getline(std::cin, password);
      std::cout << "First name: ";
      std::getline(std::cin, first_name);
      std::cout << "Last name: ";
      std::getline(std::cin, last_name);
      std::cout << "Email: ";
      std::getline(std::cin, email);
      j["username"] = username;
      j["password"] = password;
      j["first_name"] = first_name;
      j["last_name"] = last_name;
      j["email"] = email;

    } else if (action == "login") {
      std::string username, password;
      std::cout << "Username: ";
      std::getline(std::cin, username);
      std::cout << "Password: ";
      std::getline(std::cin, password);
      j["username"] = username;
      j["password"] = password;

    } else {
      std::cout << "Unknown action: " << action << "\n";
      continue;
    }

    if (!SendAndReceive(sock, j, SERVER_IP, SERVER_PORT)) {
      std::cerr << "Failed to communicate with server. Exiting.\n";
      break;
    }
  }

  close(sock);
  return EXIT_SUCCESS;
}