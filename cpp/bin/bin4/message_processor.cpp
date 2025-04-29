#include "json.hpp"
#include "message_processor.h"
#include "routes.h"

using json = nlohmann::json;

json handle_get(const json& j) {
  static const std::unordered_map<std::string, std::function<json()>>
      get_routes = {{"/api/info", handle_api_info},
                    {"/api/status", handle_api_status},
                    {"/api/user", handle_api_user}};
  std::string path = j["data"];
  auto it = get_routes.find(path);
  if (it != get_routes.end()) return it->second();
  return json{{"status", "error"}, {"message", "Unknown GET path: " + path}};
}

json handle_post(const json& j) {
  static const std::unordered_map<std::string, std::function<json(const json&)>>
      post_routes = {{"login", handle_login}, {"upload", handle_upload}};
  std::string action = j["data"].value("action", "");
  auto it = post_routes.find(action);
  if (it != post_routes.end()) return it->second(j["data"]);
  return json{{"status", "error"},
              {"message", "Unknown POST action: " + action}};
}

json handle_json(const json& j) {
  return json{{"status", "ok"},
              {"message", "Generic JSON received"},
              {"echo", j["data"]}};
}

std::string process_message(const std::string& message) {
  try {
    json j = json::parse(message);
    std::string type = j["type"];
    if (type == "GET") return handle_get(j).dump();
    if (type == "POST") return handle_post(j).dump();
    if (type == "JSON") return handle_json(j).dump();
    return json({{"status", "error"}, {"message", "Unknown type: " + type}})
        .dump();
  } catch (const std::exception& e) {
    return json({{"status", "error"},
                 {"message", std::string("Invalid JSON: ") + e.what()}})
        .dump();
  }
}