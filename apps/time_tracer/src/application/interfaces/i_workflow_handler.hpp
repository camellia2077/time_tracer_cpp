// application/interfaces/i_workflow_handler.hpp
#ifndef APPLICATION_INTERFACES_I_WORKFLOW_HANDLER_HPP_
#define APPLICATION_INTERFACES_I_WORKFLOW_HANDLER_HPP_

#include "common/app_options.hpp"
#include <map>
#include <string>
#include <vector>
// [路径修正]
#include "common/types/date_check_mode.hpp" // ValidatorUtils 可能需要拆分，这里先补上枚举定�?
#include "core/domain/model/daily_log.hpp"

// 前向声明
class AppConfig;

class IWorkflowHandler {
public:
  virtual ~IWorkflowHandler() = default;

  virtual void RunConverter(const std::string &input_path,
                            const AppOptions &options) = 0;
  virtual void RunDatabaseImport(const std::string &processed_path_str) = 0;
  virtual void RunDatabaseImportFromMemory(
      const std::map<std::string, std::vector<DailyLog>> &data_map) = 0;
  virtual void RunIngest(const std::string &source_path,
                         DateCheckMode date_check_mode,
                         bool save_processed = false) = 0;
  virtual const AppConfig &GetConfig() const = 0;
};

#endif
