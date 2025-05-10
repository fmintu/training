#include <cstdlib>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>
#include <string>

#include "controller.h"
#include "db_manager.h"
#include "socket_manager.h"

int main() {
  try {
    // 1. create database
    SqlManager db;
    if (!db.initDatabase("app.db")) {
      std::cerr << "Failed to initialize database\n";
      return EXIT_FAILURE;
    }

    // 2. init
    RequestHandler reqHandler(&db);
    SocketManager sockMgr(123456);
    Controller controller(&db, &sockMgr, &reqHandler);

    // 3. run server
    controller.Init();
    controller.Wait();

    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown fatal error occurred\n";
    return EXIT_FAILURE;
  }

  return 0;
}