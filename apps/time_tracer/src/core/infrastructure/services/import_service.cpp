// core/infrastructure/services/import_service.cpp
#include "core/infrastructure/services/import_service.hpp"
#include "importer/data_importer.hpp" 

namespace core::service {

ImportService::ImportService(std::string db_path, 
                             std::shared_ptr<core::interfaces::IFileSystem> fs,
                             std::shared_ptr<core::interfaces::IUserNotifier> notifier,
                             std::shared_ptr<core::interfaces::ILogSerializer> serializer)
    : db_path_(std::move(db_path)), 
      fs_(std::move(fs)), 
      notifier_(std::move(notifier)),
      serializer_(std::move(serializer)) {}

void ImportService::import_from_files(const std::string& directory_path) {
    notifier_->notify_info("正在扫描待导入文件...");
    
    auto json_files = fs_->resolve_file_paths({directory_path}, ".json");
    if (json_files.empty()) return;

    std::map<std::string, std::vector<DailyLog>> memory_data;

    for(const auto& filepath : json_files) {
        try {
            std::string content = fs_->read_content(filepath);
            // [修改] 使用注入的 serializer 接口
            std::vector<DailyLog> logs = serializer_->deserialize(content);
            memory_data[filepath] = logs;
        } catch (const std::exception& e) {
            notifier_->notify_error("解析文件失败 " + filepath + ": " + e.what());
        }
    }

    if (memory_data.empty()) {
        notifier_->notify_warning("没有有效的 JSON 数据可供导入。");
        return;
    }

    import_from_memory(memory_data);
}

void ImportService::import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map) {
    // 依然调用底层 Importer (暂未重构部分)
    handle_process_memory_data(db_path_, data_map);
}

}