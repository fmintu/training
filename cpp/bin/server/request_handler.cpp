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

  // Delete profile
  post_handlers_.emplace("delete_profile", [this](const json& data) {
    int id = data.value("id", 0);
    if (!db_manager_->DeleteUserProfile(id)) {
      return json{{"status", "error"}, {"message", "Failed to delete profile"}};
    }
    return json{{"status", "ok"}, {"message", "Profile deleted"}};
  });

  // ===== UserAccount CRUD =====

  post_handlers_.emplace("register_account", [this](const json& data) {
    const std::string username = data.value("username", "unknown");
    const std::string password = data.value("password", "unknown");
    const std::string first_name = data.value("first_name", "unknown");
    const std::string last_name = data.value("last_name", "unknown");
    const std::string email = data.value("email", "unknown");

    db_manager_->ExecuteSQL("BEGIN TRANSACTION;");

    int profile_id = 0;
    if (!db_manager_->AddUserProfile(first_name, last_name, email,
                                     profile_id)) {
      db_manager_->ExecuteSQL("ROLLBACK;");
      return json{{"status", "error"},
                  {"message", "Failed to create profile (email may be taken)"}};
    }

    int account_id = 0;
    if (!db_manager_->AddUserAccount(username, password, profile_id,
                                     account_id)) {
      db_manager_->ExecuteSQL("ROLLBACK;");
      return json{
          {"status", "error"},
          {"message", "Failed to create account (username may be taken)"}};
    }

    db_manager_->ExecuteSQL("COMMIT;");
    return json{{"status", "ok"},
                {"profile_id", profile_id},
                {"account_id", account_id},
                {"message", "User registered successfully"}};
  });

  // ======= PROFILE CRUD =======
  // GET PROFILE
  post_handlers_.emplace("get_profile", [this](const json& data) {
    int id = data.value("id", 0);
    if (!db_manager_->GetUserProfile(id)) {
      return json{{"status", "error"}, {"message", "Profile not found"}};
    }
    return json{{"status", "ok"},
                {"message", "Profile printed to server console"}};
  });

  // UPDATE PROFILE
  post_handlers_.emplace("update_profile", [this](const json& data) {
    const int id = data.value("id", 0);
    const std::string first_name = data.value("first_name", "unknown");
    const std::string last_name = data.value("last_name", "unknown");
    const std::string email = data.value("email", "unknown");

    if (!db_manager_->UpdateUserProfile(id, first_name, last_name, email)) {
      return json{{"status", "error"}, {"message", "Failed to update profile"}};
    }
    return json{{"status", "ok"}, {"message", "Profile updated successfully"}};
  });

  // DELETE PROFILE
  post_handlers_.emplace("delete_profile", [this](const json& data) {
    int id = data.value("id", 0);
    if (!db_manager_->DeleteUserProfile(id)) {
      return json{{"status", "error"}, {"message", "Failed to delete profile"}};
    }
    return json{{"status", "ok"}, {"message", "Profile deleted successfully"}};
  });

  // ======= ACCOUNT CRUD =======
  // GET ACCOUNT
  post_handlers_.emplace("get_account", [this](const json& data) {
    const std::string username = data.value("username", "unknown");
    if (!db_manager_->GetUserAccount(username)) {
      return json{{"status", "error"}, {"message", "Account not found"}};
    }
    return json{{"status", "ok"},
                {"message", "Account printed to server console"}};
  });

  // UPDATE PASSWORD
  post_handlers_.emplace("update_password", [this](const json& data) {
    const std::string username = data.value("username", "unknown");
    const std::string new_password = data.value("new_password", "unknown");
    if (!db_manager_->UpdateUserPassword(username, new_password)) {
      return json{{"status", "error"},
                  {"message", "Failed to update password"}};
    }
    return json{{"status", "ok"}, {"message", "Password updated successfully"}};
  });

  // DELETE ACCOUNT
  post_handlers_.emplace("delete_account", [this](const json& data) {
    int id = data.value("id", 0);
    if (!db_manager_->DeleteUserAccount(id)) {
      return json{{"status", "error"}, {"message", "Failed to delete account"}};
    }
    return json{{"status", "ok"}, {"message", "Account deleted successfully"}};
  });
}

json RequestHandler::Handle(const json& req) {
  std::string type = req.value("type", "unknown");
  if (type == "GET") {
    return HandleGet(req.value("data", "unknown"));
  } else if (type == "POST") {
    auto obj = req.value("data", json{});
    return HandlePost(obj.value("action", "unknown"), obj);
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
