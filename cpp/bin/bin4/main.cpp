#include <cstdlib>  // for EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>

#include "controller.h"
#include "db_manager.h"
#include "socket_manager.h"

int main() {
  try {
    // 1. create database
    SqlManager db;
    if (!db.initDatabase("login_history.db")) {
      std::cerr << "Failed to initialize database\n";
      return EXIT_FAILURE;
    }

    // 2. init
    SocketManager sockMgr(123456, &db);
    Controller controller(&db, &sockMgr);

    // 3. run server
    controller.Init();
    controller.Wait();

    // close database
    db.closeDatabase();

    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    std::cerr << "Unhandled exception: " << e.what() << "\n";
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown fatal error occurred\n";
    return EXIT_FAILURE;
  }
}