// core/application/utils/processed_data_writer.cpp
#include "core/application/utils/processed_data_writer.hpp"
#include "serializer/json_serializer.hpp"

namespace fs = std::filesystem;

namespace core::pipeline {

std::vector<fs::path> ProcessedDataWriter::write(
    const std::map<std::string, std::vector<DailyLog>>& data,
    const std::map<std::string, std::string>& cached_json_outputs,
    const fs::path& output_root,
    core::interfaces::IFileSystem& fs,
    core::interfaces::IUserNotifier& notifier
) {
    std::vector<fs::path> written_files;

    for (const auto& [month_key, month_days] : data) {
        fs::path output_file_path = output_root / "Processed_Date" / (month_key + ".json");
        fs::path month_output_dir = output_file_path.parent_path();

        try {
            fs.create_directories(month_output_dir);
            
            std::string content_to_write;
            auto cache_it = cached_json_outputs.find(month_key);
            if (cache_it != cached_json_outputs.end()) {
                content_to_write = cache_it->second; 
            } else {
                content_to_write = serializer::JsonSerializer::serializeDays(month_days);
            }
            
            fs.write_content(output_file_path, content_to_write);
            written_files.push_back(output_file_path);
        } catch (const std::exception& e) {
             notifier.notify_error("错误: 无法写入输出文件: " + output_file_path.string() + " - " + e.what());
        }
    }
    return written_files;
}

}