// db_manager.h
#pragma once

#include <sqlite3.h>

#include <mutex>
#include <string>
struct sqlite3;

struct UserAccount {
  int id;
  std::string username;
  std::string password;
  int profile_id;
  std::string created_at;
};

class DbManager {
 public:
  virtual ~DbManager() = default;
  virtual bool initDatabase(const std::string& filename) = 0;

  virtual void closeDatabase() = 0;

  // CRUD user_profile
  virtual bool addUserProfile(const std::string& firstName,
                              const std::string& lastName,
                              const std::string& email) = 0;
  virtual bool getUserProfile(int id) = 0;
  virtual bool updateUserProfile(int id, const std::string& firstName,
                                 const std::string& lastName,
                                 const std::string& email) = 0;
  virtual bool deleteUserProfile(int id) = 0;

  // CRUD user_account
  virtual bool addUserAccount(const std::string& username,
                              const std::string& password, int profileId) = 0;
  virtual bool getUserAccount(const std::string& username) = 0;
  virtual bool updateUserPassword(const std::string& username,
                                  const std::string& newPassword) = 0;
  virtual bool deleteUserAccount(int id) = 0;
};

class SqlManager : public DbManager {
 public:
  SqlManager();
  ~SqlManager() override;

  bool initDatabase(const std::string& filename) override;
  void closeDatabase() override;

  // CRUD user_profile
  bool addUserProfile(const std::string& firstName, const std::string& lastName,
                      const std::string& email) override;
  bool getUserProfile(int id) override;
  bool updateUserProfile(int id, const std::string& firstName,
                         const std::string& lastName,
                         const std::string& email) override;
  bool deleteUserProfile(int id) override;

  // CRUD user_account
  bool addUserAccount(const std::string& username, const std::string& password,
                      int profileId) override;
  bool getUserAccount(const std::string& username) override;
  bool updateUserPassword(const std::string& username,
                          const std::string& newPassword) override;
  bool deleteUserAccount(int id) override;

 private:
  sqlite3* db_ = nullptr;
  std::mutex db_mutex_;

  bool executeSQL(const char* sql);
  bool createAllTables();
};