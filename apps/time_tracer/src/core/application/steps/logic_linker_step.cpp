// core/application/steps/logic_linker_step.cpp
#include "core/application/steps/logic_linker_step.hpp"
#include "converter/convert/core/log_linker.hpp"

namespace core::pipeline {

bool LogicLinkerStep::execute(PipelineContext& context) {
    if (context.result.processed_data.empty()) {
        return true;
    }

    context.notifier->notify_info("Step: Linking cross-month data...");

    try {
        // [逻辑修复] 只传递 linker_config 子结构
        LogLinker linker(context.state.converter_config.linker_config);
        linker.link_logs(context.result.processed_data);
    } catch (const std::exception& e) {
        context.notifier->notify_error("[Pipeline] Logic Linker Error: " + std::string(e.what()));
        return true; 
    }

    return true;
}

}