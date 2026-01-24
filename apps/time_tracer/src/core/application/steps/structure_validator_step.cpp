// core/application/steps/structure_validator_step.cpp
#include "core/application/steps/structure_validator_step.hpp"
#include "validator/txt/facade/text_validator.hpp"
#include "validator/common/validator_utils.hpp" // [新增] 用于调用 format_error_report

namespace core::pipeline {

bool StructureValidatorStep::execute(PipelineContext& context) {
    context.notifier->notify_info("Step: Validating Source Structure (TXT)...");

    validator::txt::TextValidator validator(context.state.converter_config);
    
    bool all_valid = true;
    int files_checked = 0;

    for (const auto& file_path : context.state.source_files) {
        files_checked++;
        std::string filename = file_path.filename().string();
        
        std::string content = context.file_system->read_content(file_path);
        
        std::set<validator::Error> errors;
        
        if (!validator.validate(filename, content, errors)) {
            all_valid = false;
            
            // [关键修改] 获取错误字符串并通过 notifier 报告
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