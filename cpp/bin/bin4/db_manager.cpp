#include "db_manager.h"

#include <sqlite3.h>

#include <iostream>
#include <mutex>

static std::mutex g_db_mutex;

SqlManager::~SqlManager() { closeDatabase(); }

bool SqlManager::initDatabase(const std::string& db_path) {
  std::lock_guard<std::mutex> lock(g_db_mutex);
  if (dbHandle_) return true;

  sqlite3* db = nullptr;
  int rc = sqlite3_open(db_path.c_str(), &db);
  if (rc != SQLITE_OK) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << "\n";
    if (db) sqlite3_close(db);
    return false;
  }

  const char* sql =
      "CREATE TABLE IF NOT EXISTS login_history ("
      "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "  username TEXT NOT NULL,"
      "  login_time TEXT NOT NULL"
      ");";
  char* err = nullptr;
  rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
  if (rc != SQLITE_OK) {
    std::cerr << "Init table error: " << err << "\n";
    sqlite3_free(err);
    sqlite3_close(db);
    return false;
  }

  dbHandle_ = db;
  return true;
}

bool SqlManager::logLoginHistory(const std::string& username) {
  std::lock_guard<std::mutex> lock(g_db_mutex);
  auto* db = reinterpret_cast<sqlite3*>(dbHandle_);
  if (!db) return false;

  const char* sql =
      "INSERT INTO login_history(username, login_time) "
      "VALUES (?, datetime('now','localtime'));";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << "\n";
    return false;
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
  rc = sqlite3_step(stmt);
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    std::cerr << "Insert failed: " << sqlite3_errmsg(db) << "\n";
    return false;
  }
  return true;
}

bool SqlManager::showAllLoginHistory() {
  std::lock_guard<std::mutex> lock(g_db_mutex);
  auto* db = reinterpret_cast<sqlite3*>(dbHandle_);
  if (!db) return false;

  const char* sql =
      "SELECT id, username, login_time "
      "FROM login_history "
      "ORDER BY login_time DESC;";
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    std::cerr << "Select prepare failed: " << sqlite3_errmsg(db) << "\n";
    return false;
  }

  std::cout << "=== Login History ===\n";
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const char* user =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    const char* time =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    std::cout << id << " | " << user << " | " << time << "\n";
  }
  sqlite3_finalize(stmt);

  if (rc != SQLITE_DONE) {
    std::cerr << "Select iteration error: " << sqlite3_errmsg(db) << "\n";
    return false;
  }
  return true;
}

void SqlManager::closeDatabase() {
  std::lock_guard<std::mutex> lock(g_db_mutex);
  if (dbHandle_) {
    sqlite3_close(reinterpret_cast<sqlite3*>(dbHandle_));
    dbHandle_ = nullptr;
  }
}