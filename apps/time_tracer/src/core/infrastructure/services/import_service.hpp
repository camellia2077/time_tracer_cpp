// core/infrastructure/services/import_service.hpp
#ifndef CORE_INFRASTRUCTURE_SERVICES_IMPORT_SERVICE_HPP_
#define CORE_INFRASTRUCTURE_SERVICES_IMPORT_SERVICE_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "common/model/daily_log.hpp"
#include "core/application/interfaces/i_file_system.hpp"
#include "core/application/interfaces/i_user_notifier.hpp"

namespace core::service {

class ImportService {
public:
    ImportService(std::string db_path, 
                  std::shared_ptr<core::interfaces::IFileSystem> fs,
                  std::shared_ptr<core::interfaces::IUserNotifier> notifier);

    // 从文件系统读取 JSON 并导入
    void import_from_files(const std::string& directory_path);

    // 直接从内存导入
    void import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map);

private:
    std::string db_path_;
    std::shared_ptr<core::interfaces::IFileSystem> fs_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
};

}
#endif