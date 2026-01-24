// core/application/pipeline/context/pipeline_context.hpp
#ifndef CORE_APPLICATION_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_
#define CORE_APPLICATION_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_

#include <vector>
#include <filesystem>
#include <map>
#include <string>
#include <memory> 

#include "common/config/app_config.hpp"
#include "validator/common/validator_utils.hpp"
#include "common/model/daily_log.hpp"
#include "common/config/models/converter_config_models.hpp"
#include "core/application/interfaces/i_file_system.hpp" 
#include "core/application/interfaces/i_user_notifier.hpp" // [新增]

namespace fs = std::filesystem;

namespace core::pipeline {

struct PipelineRunConfig {
    const AppConfig& app_config;
    fs::path input_root;
    fs::path output_root;
    DateCheckMode date_check_mode = DateCheckMode::None;
    bool save_processed_output = false;

    PipelineRunConfig(const AppConfig& cfg, fs::path out) 
        : app_config(cfg), output_root(std::move(out)) {}
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

    // 核心服务接口
    std::shared_ptr<core::interfaces::IFileSystem> file_system;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier; // [新增]

    std::map<std::string, std::string> cached_json_outputs;

    PipelineContext(const AppConfig& cfg, 
                    const fs::path& out_root, 
                    std::shared_ptr<core::interfaces::IFileSystem> fs,
                    std::shared_ptr<core::interfaces::IUserNotifier> notify_svc) // [修改]
        : config(cfg, out_root), 
          file_system(std::move(fs)),
          notifier(std::move(notify_svc)) {}
};

} // namespace core::pipeline

#endif