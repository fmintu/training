#include "db_manager.h"

#include <iostream>

#include "db_define.h"

SqlManager::SqlManager() = default;

SqlManager::~SqlManager() { closeDatabase(); }

bool SqlManager::initDatabase(const std::string& filename) {
  std::lock_guard<std::mutex> lock(db_mutex_);
  if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK) {
    std::cerr << "Can't open DB: " << sqlite3_errmsg(db_) << "\n";
    return false;
  }
  return createAllTables();
}

void SqlManager::closeDatabase() {
  std::lock_guard<std::mutex> lock(db_mutex_);
  if (db_) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

bool SqlManager::executeSQL(const char* sql) {
  char* errmsg = nullptr;
  int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errmsg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << errmsg << "\n";
    sqlite3_free(errmsg);
    return false;
  }
  return true;
}

bool SqlManager::createAllTables() {
  using namespace DBSQL;
  return executeSQL(UserProfileTable::CREATE) &&
         executeSQL(UserAccountTable::CREATE);
}

// ========== USER PROFILE ==========

bool SqlManager::addUserProfile(const std::string& fn, const std::string& ln,
                                const std::string& email) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserProfileTable::INSERT, -1, &stmt, nullptr);
  sqlite3_bind_text(stmt, 1, fn.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, ln.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::getUserProfile(int id) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserProfileTable::SELECT, -1, &stmt, nullptr);
  sqlite3_bind_int(stmt, 1, id);

  int rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    std::cout << "UserProfile: " << sqlite3_column_text(stmt, 1) << " "
              << sqlite3_column_text(stmt, 2)
              << ", Email: " << sqlite3_column_text(stmt, 3) << "\n";
  }
  sqlite3_finalize(stmt);
  return rc == SQLITE_ROW;
}

bool SqlManager::updateUserProfile(int id, const std::string& fn,
                                   const std::string& ln,
                                   const std::string& email) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserProfileTable::UPDATE, -1, &stmt, nullptr);
  sqlite3_bind_text(stmt, 1, fn.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, ln.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 4, id);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::deleteUserProfile(int id) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserProfileTable::DELETE, -1, &stmt, nullptr);
  sqlite3_bind_int(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

// ========== USER ACCOUNT ==========

bool SqlManager::addUserAccount(const std::string& username,
                                const std::string& password, int profileId) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserAccountTable::INSERT, -1, &stmt, nullptr);
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, profileId);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::getUserAccount(const std::string& username) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserAccountTable::SELECT_BY_USERNAME, -1,
                     &stmt, nullptr);
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

  int rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    std::cout << "Account: " << sqlite3_column_text(stmt, 1)
              << " (profile_id: " << sqlite3_column_int(stmt, 3) << ")\n";
  }
  sqlite3_finalize(stmt);
  return rc == SQLITE_ROW;
}

bool SqlManager::updateUserPassword(const std::string& username,
                                    const std::string& newPassword) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserAccountTable::UPDATE_PASSWORD, -1, &stmt,
                     nullptr);
  sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);

  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}

bool SqlManager::deleteUserAccount(int id) {
  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db_, DBSQL::UserAccountTable::DELETE, -1, &stmt, nullptr);
  sqlite3_bind_int(stmt, 1, id);
  int rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  return rc == SQLITE_DONE;
}