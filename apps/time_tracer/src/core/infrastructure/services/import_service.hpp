// core/infrastructure/services/import_service.hpp
#ifndef CORE_INFRASTRUCTURE_SERVICES_IMPORT_SERVICE_HPP_
#define CORE_INFRASTRUCTURE_SERVICES_IMPORT_SERVICE_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "core/domain/model/daily_log.hpp"
#include "application/interfaces/i_file_system.hpp"
#include "application/interfaces/i_user_notifier.hpp"
#include "application/interfaces/i_log_serializer.hpp" // [新增] 依赖接口

namespace core::service {

class ImportService {
public:
    ImportService(std::string db_path, 
                  std::shared_ptr<core::interfaces::IFileSystem> fs,
                  std::shared_ptr<core::interfaces::IUserNotifier> notifier,
                  std::shared_ptr<core::interfaces::ILogSerializer> serializer); // [修改] 注入 Serializer

    void import_from_files(const std::string& directory_path);
    void import_from_memory(const std::map<std::string, std::vector<DailyLog>>& data_map);

private:
    std::string db_path_;
    std::shared_ptr<core::interfaces::IFileSystem> fs_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
    std::shared_ptr<core::interfaces::ILogSerializer> serializer_; // [新增] 持有接口
};

}
#endif