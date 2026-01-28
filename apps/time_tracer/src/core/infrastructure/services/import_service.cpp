// core/infrastructure/services/import_service.cpp
#include "core/infrastructure/services/import_service.hpp"
#include "importer/data_importer.hpp"

namespace core::service {

ImportService::ImportService(
    std::string db_path, std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer)
    : db_path_(std::move(db_path)), fs_(std::move(fs)),
      notifier_(std::move(notifier)), serializer_(std::move(serializer)) {}

void ImportService::ImportFromFiles(const std::string &directory_path) {
  notifier_->NotifyInfo("正在扫描待导入文件...");

  auto json_files = fs_->ResolveFilePaths({directory_path}, ".json");
  if (json_files.empty())
    return;

  std::map<std::string, std::vector<DailyLog>> memory_data;

  for (const auto &filepath : json_files) {
    try {
      std::string content = fs_->ReadContent(filepath);
      // [修改] 使用注入的 serializer 接口
      std::vector<DailyLog> logs = serializer_->Deserialize(content);
      memory_data[filepath] = logs;
    } catch (const std::exception &e) {
      notifier_->NotifyError("解析文件失败 " + filepath + ": " + e.what());
    }
  }

  if (memory_data.empty()) {
    notifier_->NotifyWarning("没有有效的 JSON 数据可供导入。");
    return;
  }

  ImportFromMemory(memory_data);
}

void ImportService::ImportFromMemory(
    const std::map<std::string, std::vector<DailyLog>> &data_map) {
  // 依然调用底层 Importer (暂未重构部分)
  handle_process_memory_data(db_path_, data_map);
}

} // namespace core::service
