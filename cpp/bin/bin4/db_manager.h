#pragma once

#include <string>

// ###################### interface ###############################
class DbManager {
 public:
  virtual ~DbManager() = default;

  virtual bool initDatabase(const std::string& db_path) = 0;
  virtual bool logLoginHistory(const std::string& username) = 0;
  virtual bool showAllLoginHistory() = 0;
  virtual void closeDatabase() = 0;
};
// ###############################################################
class SqlManager : public DbManager {
 public:
  SqlManager() = default;
  ~SqlManager() override;

  bool initDatabase(const std::string& db_path) override;
  bool logLoginHistory(const std::string& username) override;
  bool showAllLoginHistory() override;
  void closeDatabase() override;

 private:
  void* dbHandle_ = nullptr;
};

// ###############################################################
class MongoManager : public DbManager {
 public:
  bool initDatabase(const std::string& db_path) override { return false; }
  bool logLoginHistory(const std::string& username) override { return false; }
  bool showAllLoginHistory() override { return false; }
  void closeDatabase() override {}
};