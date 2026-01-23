// core/pipeline/context/pipeline_context.hpp
#ifndef CORE_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_
#define CORE_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_

#include <vector>
#include <filesystem>
#include <map>
#include <string> // [新增]

#include "common/config/app_config.hpp"
#include "validator/common/ValidatorUtils.hpp"
#include "common/model/daily_log.hpp"
#include "common/config/models/converter_config_models.hpp"

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

    // [修改] 缓存已验证的 JSON 字符串 (原为 nlohmann::json)
    std::map<std::string, std::string> cached_json_outputs;

    PipelineContext(const AppConfig& cfg, const fs::path& out_root)
        : config(cfg, out_root) {}
};

} // namespace core::pipeline

#endif // CORE_PIPELINE_CONTEXT_PIPELINE_CONTEXT_HPP_