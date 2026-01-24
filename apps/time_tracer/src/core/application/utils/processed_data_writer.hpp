// core/application/utils/processed_data_writer.hpp
#ifndef CORE_APPLICATION_UTILS_PROCESSED_DATA_WRITER_HPP_
#define CORE_APPLICATION_UTILS_PROCESSED_DATA_WRITER_HPP_

#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "common/model/daily_log.hpp"
#include "core/application/interfaces/i_file_system.hpp"
#include "core/application/interfaces/i_user_notifier.hpp" // [新增]

namespace core::pipeline {

class ProcessedDataWriter {
public:
    static std::vector<std::filesystem::path> write(
        const std::map<std::string, std::vector<DailyLog>>& data,
        const std::map<std::string, std::string>& cached_json_outputs,
        const std::filesystem::path& output_root,
        core::interfaces::IFileSystem& fs,
        core::interfaces::IUserNotifier& notifier // [新增] 引用传递
    );
};

} 
#endif