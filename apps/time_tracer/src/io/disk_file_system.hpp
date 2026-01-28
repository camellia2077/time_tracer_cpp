// io/disk_file_system.hpp
#ifndef IO_DISK_FILE_SYSTEM_HPP_
#define IO_DISK_FILE_SYSTEM_HPP_

#include "core/domain/ports/i_file_system.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace io {

/**
 * @brief IFileSystem 的磁盘实现版本�?
 * @details 这是一个“叶子节点”类，负责直接调用操作系统的文件 API
 * (std::filesystem)�? 它不应该再依�?io 模块下的其他静态工具类�?
 */
class DiskFileSystem : public core::interfaces::IFileSystem {
public:
  DiskFileSystem() = default;
  ~DiskFileSystem() override = default;

  // --- 读取操作 ---
  std::string ReadContent(const std::filesystem::path &path) override;

  // 递归查找
  std::vector<std::filesystem::path>
  FindFiles(const std::filesystem::path &root, const std::string &ext) override;

  // 解析路径列表 (展开目录)
  std::vector<std::string>
  ResolveFilePaths(const std::vector<std::string> &inputs,
                   const std::string &ext) override;

  // --- 写入操作 ---
  void WriteContent(const std::filesystem::path &path,
                    const std::string &content) override;
  void CreateDirectories(const std::filesystem::path &path) override;

  // --- 状态检�?---
  bool Exists(const std::filesystem::path &path) override;
  bool IsDirectory(const std::filesystem::path &path) override;
  bool IsRegularFile(const std::filesystem::path &path) override;
};

} // namespace io

#endif // IO_DISK_FILE_SYSTEM_HPP_
