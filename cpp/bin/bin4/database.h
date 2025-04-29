#pragma once
#include <string>

bool init_database(const std::string& db_path);
bool log_login_history(const std::string& username);
bool show_all_login_history();
void close_database();