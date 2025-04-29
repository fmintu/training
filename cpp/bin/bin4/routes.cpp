#include "routes.h"

#include "database.h"

json handle_api_info() {
  return json{{"status", "ok"}, {"message", "This is API info"}};
}

json handle_api_status() {
  return json{{"status", "ok"}, {"message", "This is API status"}};
}

json handle_api_user() {
  return json{{"status", "ok"}, {"message", "This is API user"}};
}

json handle_login(const json& data) {
  std::string username = data.value("username", "unknown");
  {
    log_login_history(username);  // SQLite
    show_all_login_history();     // SQLite
  }
  return json{{"status", "ok"}, {"message", "User " + username + " logged in"}};
}

json handle_upload(const json& data) {
  return json{{"status", "ok"}, {"message", "Upload received: " + data.dump()}};
}