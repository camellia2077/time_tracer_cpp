// core/domain/ports/i_file_system.hpp
#ifndef CORE_DOMAIN_PORTS_I_FILE_SYSTEM_HPP_
#define CORE_DOMAIN_PORTS_I_FILE_SYSTEM_HPP_

#include <filesystem>
#include <string>
#include <vector>

namespace core::interfaces {

class IFileSystem {
public:
  virtual ~IFileSystem() = default;

  // --- 读取操作 ---
  virtual std::string ReadContent(const std::filesystem::path &path) = 0;

  // 递归查找指定后缀的文�?
  virtual std::vector<std::filesystem::path>
  FindFiles(const std::filesystem::path &root, const std::string &ext) = 0;

  // 解析混合路径列表（文�?目录），返回所有符合后缀的文件路�?
  virtual std::vector<std::string>
  ResolveFilePaths(const std::vector<std::string> &inputs,
                   const std::string &ext) = 0;

  // --- 写入操作 ---
  virtual void WriteContent(const std::filesystem::path &path,
                            const std::string &content) = 0;
  virtual void CreateDirectories(const std::filesystem::path &path) = 0;

  // --- 状态检�?---
  virtual bool Exists(const std::filesystem::path &path) = 0;
  virtual bool IsDirectory(const std::filesystem::path &path) = 0;
  virtual bool IsRegularFile(const std::filesystem::path &path) = 0;
};

} // namespace core::interfaces
#endif
