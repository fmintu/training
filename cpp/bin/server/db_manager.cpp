#include "db_manager.h"

#include <iostream>
#include <mutex>

#include "db_define.h"
#include "db_manager.h"

SqlManager::SqlManager() = default;

SqlManager::~SqlManager() { CloseDatabase(); }

bool SqlManager::InitDatabase(const std::string& filename) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << "\n";
    return false;
  }
  return CreateAllTables();
}

void SqlManager::CloseDatabase() {
  std::lock_guard<std::mutex> lock(db_mutex_);
  if (db_ != nullptr) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

bool SqlManager::ExecuteSQL(const char* sql) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  char* errmsg = nullptr;
  int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errmsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << errmsg << "\n";
    sqlite3_free(errmsg);
    return false;
  }
  return true;
}

bool SqlManager::CreateAllTables() {
  return ExecuteSQL(SQL_CREATE_USER_PROFILE_TABLE) &&
         ExecuteSQL(SQL_CREATE_USER_ACCOUNT_TABLE);
}

bool SqlManager::AddUserProfile(const std::string& first_name,
                                const std::string& last_name,
                                const std::string& email, int& out_profile_id) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_INSERT_USER_PROFILE, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, last_name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) return false;

  out_profile_id = static_cast<int>(sqlite3_last_insert_rowid(db_));
  return true;
}

bool SqlManager::AddUserAccount(const std::string& username,
                                const std::string& password, int profile_id,
                                int& out_account_id) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_INSERT_USER_ACCOUNT, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, profile_id);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (rc != SQLITE_DONE) return false;

  out_account_id = static_cast<int>(sqlite3_last_insert_rowid(db_));
  return true;
}

bool SqlManager::GetUserProfile(int id) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_SELECT_USER_PROFILE, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_int(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    std::cout << "UserProfile: "
              << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
              << " "
              << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
              << ", Email: "
              << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
              << "\n";
  }
  sqlite3_finalize(stmt);
  return rc == SQLITE_ROW;
}

bool SqlManager::UpdateUserProfile(int id, const std::string& first_name,
                                   const std::string& last_name,
                                   const std::string& email) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_UPDATE_USER_PROFILE, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_text(stmt, 1, first_name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, last_name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 4, id);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::GetUserAccount(const std::string& username) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_SELECT_USER_ACCOUNT_BY_USERNAME, -1, &stmt,
                     nullptr);
  if (!stmt) return false;

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
  int rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    std::cout << "Account: "
              << reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))
              << " (profile_id: " << sqlite3_column_int(stmt, 3) << ")\n";
  }
  sqlite3_finalize(stmt);
  return rc == SQLITE_ROW;
}

bool SqlManager::UpdateUserPassword(const std::string& username,
                                    const std::string& new_password) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_UPDATE_USER_ACCOUNT_PASSWORD, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_text(stmt, 1, new_password.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::DeleteUserProfile(int id) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_DELETE_USER_PROFILE, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_int(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::DeleteUserAccount(int id) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_, SQL_DELETE_USER_ACCOUNT, -1, &stmt, nullptr);
  if (!stmt) return false;

  sqlite3_bind_int(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

int SqlManager::GetLastInsertRowId() {
  std::lock_guard<std::mutex> lock(db_mutex_);
  return static_cast<int>(sqlite3_last_insert_rowid(db_));
}