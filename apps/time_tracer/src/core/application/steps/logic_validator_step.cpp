// core/application/steps/logic_validator_step.cpp
#include "core/application/steps/logic_validator_step.hpp"
#include "validator/json/facade/json_validator.hpp"
#include "validator/common/validator_utils.hpp" // [新增]
#include "serializer/json_serializer.hpp" 

namespace core::pipeline {

bool LogicValidatorStep::execute(PipelineContext& context) {
    context.notifier->notify_info("Step: Validating Business Logic (Dates, Continuity)...");

    if (context.result.processed_data.empty()) {
        context.notifier->notify_warning("No data to validate.");
        return true;
    }

    validator::json::JsonValidator validator(context.config.date_check_mode);
    bool all_valid = true;

    for (const auto& [month_key, days] : context.result.processed_data) {
        if (days.empty()) continue;

        // 使用缓存或序列化
        std::string json_content;
        auto it = context.cached_json_outputs.find(month_key);
        if (it != context.cached_json_outputs.end()) {
            json_content = it->second;
        } else {
            json_content = serializer::JsonSerializer::serializeDays(days);
            context.cached_json_outputs[month_key] = json_content;
        }

        std::string pseudo_filename = "ProcessedData[" + month_key + "]";
        std::set<validator::Error> errors;
        
        if (!validator.validate(pseudo_filename, json_content, errors)) {
            all_valid = false;
            
            // [关键修改] 格式化并报告
            std::string error_report = validator::format_error_report(pseudo_filename, errors);
            context.notifier->notify_error(error_report);
        }
    }

    if (all_valid) {
        context.notifier->notify_success("Logic validation passed.");
    } else {
        context.notifier->notify_error("Logic validation found issues (e.g., broken date continuity).");
    }

    return all_valid;
}

}