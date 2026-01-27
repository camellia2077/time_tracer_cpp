// application/steps/structure_validator_step.cpp
#include "application/steps/structure_validator_step.hpp"
#include "validator/txt/facade/text_validator.hpp"
#include "validator/common/validator_utils.hpp" // 用于调用 format_error_report
#include <set>

namespace core::pipeline {

bool StructureValidatorStep::execute(PipelineContext& context) {
    context.notifier->notify_info("Step: Validating Source Structure (TXT)...");

    // [修复] 变量名改�?text_validator，避免与 namespace validator 冲突
    validator::txt::TextValidator text_validator(context.state.converter_config);
    
    bool all_valid = true;
    int files_checked = 0;

    for (const auto& file_path : context.state.source_files) {
        files_checked++;
        std::string filename = file_path.filename().string();
        
        // 读取文件内容
        std::string content;
        try {
            content = context.file_system->read_content(file_path);
        } catch (const std::exception& e) {
            context.notifier->notify_error("Failed to read file: " + filename + " - " + e.what());
            all_valid = false;
            continue;
        }
        
        std::set<validator::Error> errors;
        
        // [修复] 使用 text_validator 实例调用
        if (!text_validator.validate(filename, content, errors)) {
            all_valid = false;
            
            // [修复] 正确调用命名空间函数 validator::format_error_report
            std::string error_report = validator::format_error_report(filename, errors);
            context.notifier->notify_error(error_report);
        }
    }

    if (all_valid) {
         context.notifier->notify_success("Structure validation passed for " + std::to_string(files_checked) + " files.");
    } else {
         context.notifier->notify_error("Structure validation failed. Please fix the errors above.");
    }

    return all_valid;
}

}