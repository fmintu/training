// db_manager.h
#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include <sqlite3.h>

#include <mutex>
#include <string>

#include "request_handler.h"
// Interface for database operations.
class DbManager {
 public:
  virtual ~DbManager() = default;

  virtual bool InitDatabase(const std::string& filename) = 0;
  virtual void CloseDatabase() = 0;
  virtual bool ExecuteSQL(const char* sql) = 0;

  virtual bool AddUserProfile(const std::string& first_name,
                              const std::string& last_name,
                              const std::string& email,
                              int& out_profile_id) = 0;

  virtual bool AddUserAccount(const std::string& username,
                              const std::string& password, int profile_id,
                              int& out_account_id) = 0;

  virtual bool GetUserProfile(int id) = 0;
  virtual bool GetUserAccount(const std::string& username) = 0;
  virtual bool UpdateUserProfile(int id, const std::string& first_name,
                                 const std::string& last_name,
                                 const std::string& email) = 0;
  virtual bool UpdateUserPassword(const std::string& username,
                                  const std::string& new_password) = 0;
  virtual bool DeleteUserProfile(int id) = 0;
  virtual bool DeleteUserAccount(int id) = 0;
  virtual int GetLastInsertRowId() = 0;
};

// SQLite implementation of DbManager.
class SqlManager : public DbManager {
 public:
  SqlManager();
  ~SqlManager() override;

  bool InitDatabase(const std::string& filename) override;
  void CloseDatabase() override;
  bool ExecuteSQL(const char* sql) override;

  bool AddUserProfile(const std::string& first_name,
                      const std::string& last_name, const std::string& email,
                      int& out_profile_id) override;
  bool AddUserAccount(const std::string& username, const std::string& password,
                      int profile_id, int& out_account_id) override;

  bool GetUserProfile(int id) override;
  bool GetUserAccount(const std::string& username) override;
  bool UpdateUserProfile(int id, const std::string& first_name,
                         const std::string& last_name,
                         const std::string& email) override;
  bool UpdateUserPassword(const std::string& username,
                          const std::string& new_password) override;
  bool DeleteUserProfile(int id) override;
  bool DeleteUserAccount(int id) override;
  int GetLastInsertRowId();

 private:
  bool CreateAllTables();

  sqlite3* db_ = nullptr;
  std::mutex db_mutex_;
};

#endif  // DB_MANAGER_H_