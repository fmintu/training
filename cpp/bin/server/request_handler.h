#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "json.hpp"

using json = nlohmann::json;
using GetHandler = std::function<json()>;
using PostHandler = std::function<json(const json&)>;

class DbManager;  // forward declaration

class RequestHandler {
 public:
  explicit RequestHandler(DbManager* dbmgr);
  json Handle(const json& req);

 private:
  void RegisterHandlers();
  json HandleGet(const std::string& path);
  json HandlePost(const std::string& action, const json& data);

  DbManager* db_manager_;
  std::unordered_map<std::string, GetHandler> get_handlers_;
  std::unordered_map<std::string, PostHandler> post_handlers_;
};
