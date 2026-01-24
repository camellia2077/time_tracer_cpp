// core/infrastructure/services/import_service.cpp
#include "core/infrastructure/services/import_service.hpp"
#include "importer/data_importer.hpp" 
#include "serializer/json_serializer.hpp"

namespace core::service {

ImportService::ImportService(std::string db_path, 
                             std::shared_ptr<core::interfaces::IFileSystem> fs,
                             std::shared_ptr<core::interfaces::IUserNotifier> notifier)
    : db_path_(std::move(db_path)), fs_(std::move(fs)), notifier_(std::move(notifier)) {}

void ImportService::import_from_files(const std::string& directory_path) {
    notifier_->notify_info("正在扫描待导入文件...");
    
    auto json_files = fs_->resolve_file_paths({directory_path}, ".json");
    if (json_files.empty()) return;

    std::map<std::string, std::vector<DailyLog>> memory_data;

    for(const auto& filepath : json_files) {
        try {
            std::string content = fs_->read_content(filepath);
            std::vector<DailyLog> logs = serializer::JsonSerializer::deserializeDays(content);
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
    // 调用底层 Importer 模块
    // 注意：此处保留了对全局/静态函数的调用，这部分可以进一步通过接口解耦，但不在本次重构范围内
    handle_process_memory_data(db_path_, data_map);
}

}