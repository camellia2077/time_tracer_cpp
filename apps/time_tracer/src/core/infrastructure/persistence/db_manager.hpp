// core/infrastructure/persistence/db_manager.hpp
#ifndef CORE_INFRASTRUCTURE_PERSISTENCE_DB_MANAGER_HPP_
#define CORE_INFRASTRUCTURE_PERSISTENCE_DB_MANAGER_HPP_

#include <filesystem>
#include <string>
struct sqlite3;

class DBManager {
public:
  explicit DBManager(const std::string &db_name);
  ~DBManager();

  DBManager(const DBManager &) = delete;
  DBManager &operator=(const DBManager &) = delete;

  // [修改] 发生错误时抛出异常，而不是打印日志
  void OpenDatabase();

  // 尝试打开，如果不存在则抛出特定异常或返回false (保留原有语义但移除打印)
  bool CheckAndOpen();

  void CloseDatabase();
  sqlite3 *GetDbConnection() const;

private:
  std::string db_name_;
  sqlite3 *db_;
};

#endif // CORE_INFRASTRUCTURE_PERSISTENCE_DB_MANAGER_HPP_
