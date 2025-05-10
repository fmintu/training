#pragma once

namespace DBSQL {

struct UserProfileTable {
  static constexpr const char* CREATE =
      "CREATE TABLE IF NOT EXISTS user_profile ( "
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "first_name TEXT NOT NULL, "
      "last_name TEXT NOT NULL, "
      "email TEXT UNIQUE NOT NULL, "
      "created_at DATETIME DEFAULT CURRENT_TIMESTAMP);";

  static constexpr const char* INSERT =
      "INSERT INTO user_profile (first_name, last_name, email) VALUES (?, ?, "
      "?);";

  static constexpr const char* DELETE =
      "DELETE FROM user_profile WHERE id = ?;";

  static constexpr const char* UPDATE =
      "UPDATE user_profile SET first_name = ?, last_name = ?, email = ? WHERE "
      "id = ?;";

  static constexpr const char* SELECT =
      "SELECT id, first_name, last_name, email, created_at FROM user_profile "
      "WHERE id = ?;";
};

struct UserAccountTable {
  static constexpr const char* CREATE =
      "CREATE TABLE IF NOT EXISTS user_account ( "
      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
      "username TEXT UNIQUE NOT NULL, "
      "password TEXT NOT NULL, "
      "profile_id INTEGER, "
      "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
      "FOREIGN KEY(profile_id) REFERENCES user_profile(id));";

  static constexpr const char* INSERT =
      "INSERT INTO user_account (username, password, profile_id) VALUES (?, ?, "
      "?);";

  static constexpr const char* DELETE =
      "DELETE FROM user_account WHERE id = ?;";

  static constexpr const char* UPDATE_PASSWORD =
      "UPDATE user_account SET password = ? WHERE username = ?;";

  static constexpr const char* SELECT_BY_USERNAME =
      "SELECT id, username, password, profile_id, created_at FROM user_account "
      "WHERE username = ?;";
};
}  // namespace DBSQL