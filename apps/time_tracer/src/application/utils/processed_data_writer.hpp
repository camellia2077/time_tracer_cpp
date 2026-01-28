// application/utils/processed_data_writer.hpp
#ifndef APPLICATION_UTILS_PROCESSED_DATA_WRITER_HPP_
#define APPLICATION_UTILS_PROCESSED_DATA_WRITER_HPP_

#include "application/interfaces/i_log_serializer.hpp" // [新增]
#include "core/domain/model/daily_log.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include "core/domain/ports/i_user_notifier.hpp"
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace core::pipeline {

class ProcessedDataWriter {
public:
  static std::vector<std::filesystem::path>
  Write(const std::map<std::string, std::vector<DailyLog>> &data,
        const std::filesystem::path &output_root,
        core::interfaces::IFileSystem &fs,
        core::interfaces::IUserNotifier &notifier,
        core::interfaces::ILogSerializer &serializer // [新增]
  );
};

} // namespace core::pipeline
#endif
