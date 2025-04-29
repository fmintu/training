#include "client_handler.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "connection_table.h"
#include "message_processor.h"

void client_handler(int client_socket, sockaddr_in client_addr) {
  std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << ":"
            << ntohs(client_addr.sin_port) << "\n";

  char buffer[4096];
  while (true) {
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
      std::cout << "Client disconnected.\n";
      std::lock_guard<std::mutex> lock(table_mutex);
      connection_table[client_socket].active = false;
      break;
    }

    std::string received(buffer);
    std::string response = process_message(received);
    send(client_socket, response.c_str(), response.size(), 0);
  }
  close(client_socket);
}