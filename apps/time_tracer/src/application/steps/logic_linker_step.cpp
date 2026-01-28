// application/steps/logic_linker_step.cpp
#include "application/steps/logic_linker_step.hpp"
#include "converter/convert/core/log_linker.hpp"

namespace core::pipeline {

bool LogicLinkerStep::Execute(PipelineContext &context) {
  if (context.result.processed_data.empty()) {
    return true;
  }

  context.notifier->NotifyInfo("Step: Linking cross-month data...");

  try {
    // [逻辑修复] 只传递 linker_config_ 子结构
    LogLinker linker(context.state.converter_config.linker_config_);
    linker.LinkLogs(context.result.processed_data);
  } catch (const std::exception &e) {
    context.notifier->NotifyError("[Pipeline] Logic Linker Error: " +
                                  std::string(e.what()));
    return true;
  }

  return true;
}

} // namespace core::pipeline
