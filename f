#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>

#include "client_handler.h"
#include "connection_table.h"
#include "database.h"

std::unordered_map<int, ClientConnection> connection_table;
std::mutex table_mutex;

int main() {
  if (!init_database("login.db")) {
    std::cerr << "Database initialization failed.\n";
    return 1;
  }
  // =============================================
  const int PORT = 123456;
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    std::cerr << "Socket creation failed.\n";
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Bind failed.\n";
    return 1;
  }

  if (listen(server_socket, SOMAXCONN) < 0) {
    std::cerr << "Listen failed.\n";
    return 1;
  }

  std::cout << "Server listening on port " << PORT << "...\n";

  while (true) {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_socket = accept(server_socket, (sockaddr*)&client_addr, &len);
    if (client_socket < 0) {
      std::cerr << "Accept failed.\n";
      continue;
    }

    std::thread t(client_handler, client_socket, client_addr);
    {
      std::lock_guard<std::mutex> lock(table_mutex);
      connection_table[client_socket] =
          ClientConnection{client_socket, std::move(t), true};
    }
  }

  close(server_socket);
  return 0;
}