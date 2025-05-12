#include "database.h"

#include <sqlite3.h>

#include <iostream>
#include <mutex>

static std::mutex db_mutex;
static sqlite3* db = nullptr;

bool init_database(const std::string& db_path) {
  int rc = sqlite3_open(db_path.c_str(), &db);
  if (rc != SQLITE_OK) {
    std::cerr << "Cannot open DB: " << sqlite3_errmsg(db) << "\n";
    return false;
  }

  const char* create_table_sql =
      "CREATE TABLE IF NOT EXISTS login_history ("
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "username TEXT NOT NULL,"
      "login_time DATETIME DEFAULT CURRENT_TIMESTAMP);";

  char* err_msg = nullptr;
  rc = sqlite3_exec(db, create_table_sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << err_msg << "\n";
    sqlite3_free(err_msg);
    return false;
  }

  return true;
}

bool log_login_history(const std::string& username) {
  std::string sql = "INSERT INTO login_history(username) VALUES(?);";
  sqlite3_stmt* stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Failed to prepare statement\n";
    return false;
  }

  {  // lock_guard
    std::lock_guard<std::mutex> lock(db_mutex);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
  }

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Insert failed\n";
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

bool show_all_login_history() {
  const char* sql =
      "SELECT id, username, login_time "
      "FROM login_history "
      "ORDER BY login_time DESC;";

  sqlite3_stmt* stmt = nullptr;
  {  // lock_guard
    std::lock_guard<std::mutex> lock(db_mutex);

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
      std::cerr << "Failed to prepare SELECT: " << sqlite3_errmsg(db) << "\n";
      return false;
    }

    std::cout << "=== Login History ===\n";
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      int id = sqlite3_column_int(stmt, 0);
      auto user = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      auto t = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      std::cout << id << " | " << user << " | " << t << "\n";
    }

    if (rc != SQLITE_DONE) {
      std::cerr << "Step error: " << sqlite3_errmsg(db) << "\n";
      sqlite3_finalize(stmt);
      return false;
    }

    sqlite3_finalize(stmt);
  }  // end lock

  return true;
}

void close_database() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
  }
}