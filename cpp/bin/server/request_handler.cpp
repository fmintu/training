#include "request_handler.h"

#include <iostream>

#include "db_manager.h"

RequestHandler::RequestHandler(DbManager* dbmgr) : db_manager_(dbmgr) {
  RegisterHandlers();
}

void RequestHandler::RegisterHandlers() {
  // GET
  get_handlers_.emplace("/api/info", []() {
    return json{{"status", "ok"}, {"message", "This is API info"}};
  });
  get_handlers_.emplace("/api/status", []() {
    return json{{"status", "ok"}, {"message", "This is API status"}};
  });
  get_handlers_.emplace("/api/user", []() {
    return json{{"status", "ok"}, {"message", "This is API user"}};
  });

  // POST
  post_handlers_.emplace("login", [this](const json& data) {
    std::string user = data.value("username", "unknown");
    std::cout << "Writing to database: " << user << std::endl;
    // dbManager_->LogLoginHistory(user);
    // dbManager_->ShowAllLoginHistory();
    return json{{"status", "ok"}, {"message", "User " + user + " logged in"}};
  });

  post_handlers_.emplace("upload", [](const json& data) {
    return json{{"status", "ok"},
                {"message", "Upload received: " + data.dump()}};
  });

  post_handlers_.emplace("register_account", [this](const json& data) {
    std::string username = data.value("username", "");
    std::string password = data.value("password", "");
    std::string fn = "";
    std::string ln = "";
    std::string email = "";

    // if (!db_manager_->addUserAccount(username, password, 1)) {
    //   return json{{"status", "ok"}, {"message", "failed "}};
    // };

    return json{{"status", "ok"},
                {"message", "User " + username + " registered "}};
  });

  post_handlers_.emplace("change_password", [this](const json& data) {
    std::string username = data.value("username", "");
    std::string password = data.value("password", "");

    // if (db_manager_->updateUserPassword(username, password)) {
    //   std::cout << "Password changed\n";
    // } else {
    //   std::cout << "Change failed\n";
    //   return json{{"status", "ok"}, {"message", "Change failed"}};
    // }

    return json{{"status", "ok"},
                {"message", "User Password: " + username + " changed"}};
  });
}

json RequestHandler::Handle(const json& req) {
  std::string type = req.value("type", "");
  if (type == "GET") {
    return HandleGet(req.value("data", ""));
  } else if (type == "POST") {
    auto obj = req.value("data", json{});
    return HandlePost(obj.value("action", ""), obj);
  } else if (type == "JSON") {
    return json{
        {"status", "ok"}, {"message", "Generic JSON"}, {"echo", req["data"]}};
  }
  return json{{"status", "error"}, {"message", "Unknown type"}};
}

json RequestHandler::HandleGet(const std::string& path) {
  auto it = get_handlers_.find(path);
  if (it != get_handlers_.end()) return it->second();
  return json{{"status", "error"}, {"message", "No GET handler for " + path}};
}

json RequestHandler::HandlePost(const std::string& action, const json& data) {
  auto it = post_handlers_.find(action);
  if (it != post_handlers_.end()) return it->second(data);
  return json{{"status", "error"},
              {"message", "No POST handler for " + action}};
}
