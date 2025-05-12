#pragma once
#include <netinet/in.h>

void client_handler(int client_socket, sockaddr_in client_addr);