// core/pipeline/steps/logic_validator_step.cpp
#include "core/pipeline/steps/logic_validator_step.hpp"
#include "validator/json/facade/json_validator.hpp"
#include "serializer/json_serializer.hpp" 
#include "common/ansi_colors.hpp"
#include <iostream>

namespace core::pipeline {

bool LogicValidatorStep::execute(PipelineContext& context) {
    std::cout << "Step: Validating Business Logic (Dates, Continuity)..." << std::endl;

    if (context.result.processed_data.empty()) {
        std::cout << YELLOW_COLOR << "No data to validate." << RESET_COLOR << std::endl;
        return true;
    }

    // 初始化 JSON 验证器
    validator::json::JsonValidator validator(context.config.date_check_mode);
    
    bool all_valid = true;

    // 遍历内存中的数据 (按月分组)
    for (const auto& [month_key, days] : context.result.processed_data) {
        if (days.empty()) continue;

        // [修改] 序列化为字符串 (yyjson backend)
        std::string json_content = serializer::JsonSerializer::serializeDays(days);
        
        // [新增] 缓存序列化结果，供后续 Write 步骤直接使用，避免二次序列化
        context.cached_json_outputs[month_key] = json_content;

        // 构造一个虚拟文件名用于报错显示
        std::string pseudo_filename = "ProcessedData[" + month_key + "]";

        std::set<validator::Error> errors;
        
        // [假设] JsonValidator::validate 接口已更新为接收 const std::string&
        if (!validator.validate(pseudo_filename, json_content, errors)) {
            all_valid = false;
            validator::printGroupedErrors(pseudo_filename, errors);
        }
    }

    if (all_valid) {
        std::cout << GREEN_COLOR << "Logic validation passed." << RESET_COLOR << std::endl;
    } else {
        std::cerr << RED_COLOR << "Logic validation found issues (e.g., broken date continuity)." << RESET_COLOR << std::endl;
    }

    return all_valid;
}

} // namespace core::pipeline