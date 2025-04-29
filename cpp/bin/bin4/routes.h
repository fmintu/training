#pragma once
#include "json.hpp"

using json = nlohmann::json;

json handle_api_info();
json handle_api_status();
json handle_api_user();
json handle_login(const json& data);
json handle_upload(const json& data);