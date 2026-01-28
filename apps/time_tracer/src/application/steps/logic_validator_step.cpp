// application/steps/logic_validator_step.cpp
#include "application/steps/logic_validator_step.hpp"
#include "validator/common/validator_utils.hpp"
#include "validator/logic/facade/logic_validator.hpp" // 引入新的 LogicValidator
#include <set>

namespace core::pipeline {

bool LogicValidatorStep::Execute(PipelineContext &context) {
  // 检查是否有关联配置，并且只在 Strong 检查模式才执行?
  // 根据需求，这里假设 validate_logic 为 true 就会执行
  context.notifier->NotifyInfo(
      "Step: Logic Validator"); // logic validator ?Logic (Struct-based)...");

  if (context.result.processed_data.empty()) {
    context.notifier->NotifyWarning("No data to validate.");
    return true;
  }

  // [修复] 变量名改logic_validator，避免冲突
  validator::logic::LogicValidator logic_validator(
      context.config.date_check_mode_);
  bool all_valid = true;

  // 遍历处理后的数据 (按月分组)
  for (const auto &[month_key, days] : context.result.processed_data) {
    if (days.empty())
      continue;

    // 执行验证 (直接传入 DailyLog 列表)
    std::set<validator::Error> errors;

    // month_key 通常"YYYY-MM"，用作上下文名称
    if (!logic_validator.validate(month_key, days, errors)) {
      all_valid = false;

      // 报告错误
      std::string context_name = "DataGroup[" + month_key + "]";
      // [修复] 正确调用命名空间函数
      std::string error_report =
          validator::format_error_report(context_name, errors);
      context.notifier->NotifyError(error_report);
    }
  }

  if (all_valid) {
    context.notifier->NotifySuccess("Logic validation passed.");
  } else {
    context.notifier->NotifyError("Logic validation found issues.");
  }

  return all_valid;
}

} // namespace core::pipeline
