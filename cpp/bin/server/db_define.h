// db_define.h
#ifndef DB_DEFINE_H_
#define DB_DEFINE_H_

// SQL statements for user_profile table.
#define SQL_CREATE_USER_PROFILE_TABLE         \
  "CREATE TABLE IF NOT EXISTS user_profile (" \
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "    \
  "first_name TEXT NOT NULL, "                \
  "last_name TEXT NOT NULL, "                 \
  "email TEXT UNIQUE NOT NULL, "              \
  "created_at DATETIME DEFAULT CURRENT_TIMESTAMP);"

#define SQL_INSERT_USER_PROFILE \
  "INSERT INTO user_profile (first_name, last_name, email) VALUES (?, ?, ?);"

#define SQL_DELETE_USER_PROFILE "DELETE FROM user_profile WHERE id = ?;"

#define SQL_UPDATE_USER_PROFILE                                                \
  "UPDATE user_profile SET first_name = ?, last_name = ?, email = ? WHERE id " \
  "= ?;"

#define SQL_SELECT_USER_PROFILE                                            \
  "SELECT id, first_name, last_name, email, created_at FROM user_profile " \
  "WHERE id = ?;"

// SQL statements for user_account table.
#define SQL_CREATE_USER_ACCOUNT_TABLE               \
  "CREATE TABLE IF NOT EXISTS user_account ("       \
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "          \
  "username TEXT UNIQUE NOT NULL, "                 \
  "password TEXT NOT NULL, "                        \
  "profile_id INTEGER, "                            \
  "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, " \
  "FOREIGN KEY(profile_id) REFERENCES user_profile(id));"

#define SQL_INSERT_USER_ACCOUNT                                              \
  "INSERT INTO user_account (username, password, profile_id) VALUES (?, ?, " \
  "?);"

#define SQL_DELETE_USER_ACCOUNT "DELETE FROM user_account WHERE id = ?;"

#define SQL_UPDATE_USER_ACCOUNT_PASSWORD \
  "UPDATE user_account SET password = ? WHERE username = ?;"

#define SQL_SELECT_USER_ACCOUNT_BY_USERNAME                                  \
  "SELECT id, username, password, profile_id, created_at FROM user_account " \
  "WHERE username = ?;"

#endif  // DB_DEFINE_H_