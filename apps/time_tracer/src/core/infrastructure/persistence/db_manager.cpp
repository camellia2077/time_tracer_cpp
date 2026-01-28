// core/infrastructure/persistence/db_manager.cpp
#include "core/infrastructure/persistence/db_manager.hpp"
#include <filesystem>
#include <sqlite3.h>
#include <stdexcept>

namespace fs = std::filesystem;

DBManager::DBManager(const std::string &db_name)
    : db_name_(db_name), db_(nullptr) {}

DBManager::~DBManager() { CloseDatabase(); }

// 对应原来的 open_database_if_needed，但更严谨
bool DBManager::CheckAndOpen() {
  if (db_ != nullptr)
    return true;

  if (!fs::exists(db_name_)) {
    // [修改] 不打印错误，由调用者处理文件不存在的情况
    return false;
  }

  if (sqlite3_open(db_name_.c_str(), &db_)) {
    std::string err_msg = sqlite3_errmsg(db_);
    sqlite3_close(db_);
    db_ = nullptr;
    // [修改] 抛出异常
    throw std::runtime_error("Failed to open database: " + err_msg);
  }
  return true;
}

void DBManager::OpenDatabase() {
  if (db_ != nullptr)
    return;

  // 如果需要创建模式，通常也是用 sqlite3_open
  if (sqlite3_open(db_name_.c_str(), &db_)) {
    std::string err_msg = sqlite3_errmsg(db_);
    sqlite3_close(db_);
    db_ = nullptr;
    throw std::runtime_error("Failed to open/create database: " + err_msg);
  }
}

void DBManager::CloseDatabase() {
  if (db_) {
    sqlite3_close(db_);
    db_ = nullptr;
  }
}

sqlite3 *DBManager::GetDbConnection() const { return db_; }
