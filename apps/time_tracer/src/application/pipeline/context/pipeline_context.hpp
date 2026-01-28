// application/pipeline/context/pipeline_context.hpp
#ifndef APPLICATION_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_
#define APPLICATION_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/config/app_config.hpp"
#include "common/config/models/converter_config_models.hpp"
#include "core/domain/model/daily_log.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include "core/domain/ports/i_user_notifier.hpp"

namespace fs = std::filesystem;

namespace core::pipeline {

struct PipelineRunConfig {
  const AppConfig &app_config_;
  fs::path input_root_;
  fs::path output_root_;
  DateCheckMode date_check_mode_ = DateCheckMode::None;
  bool save_processed_output_ = false;

  PipelineRunConfig(const AppConfig &cfg, fs::path out)
      : app_config_(cfg), output_root_(std::move(out)) {}
};

struct PipelineState {
  std::vector<fs::path> source_files;
  std::vector<fs::path> generated_files;
  ConverterConfig converter_config;
};

struct PipelineResult {
  std::map<std::string, std::vector<DailyLog>> processed_data;
};

class PipelineContext {
public:
  PipelineRunConfig config;
  PipelineState state;
  PipelineResult result;

  std::shared_ptr<core::interfaces::IFileSystem> file_system;
  std::shared_ptr<core::interfaces::IUserNotifier> notifier;

  // [修改] 移除�?cached_json_outputs
  // 验证阶段不再负责序列化，序列化工作完全由 Writer 步骤负责，职责更清晰�?

  PipelineContext(const AppConfig &cfg, const fs::path &out_root,
                  std::shared_ptr<core::interfaces::IFileSystem> fs,
                  std::shared_ptr<core::interfaces::IUserNotifier> notify_svc)
      : config(cfg, out_root), file_system(std::move(fs)),
        notifier(std::move(notify_svc)) {}
};

} // namespace core::pipeline

#endif
