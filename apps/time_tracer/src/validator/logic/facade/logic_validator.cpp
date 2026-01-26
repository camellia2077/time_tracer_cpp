// validator/logic/facade/logic_validator.cpp
#include "validator/logic/facade/logic_validator.hpp"
#include "validator/logic/rules/date_rules.hpp"
#include "validator/logic/rules/activity_rules.hpp"

namespace validator {
namespace logic {

LogicValidator::LogicValidator(DateCheckMode date_check_mode)
    : date_check_mode_(date_check_mode) {}

bool LogicValidator::validate(const std::string& /*context_name*/, const std::vector<DailyLog>& days, std::set<Error>& errors) {
    // 1. 验证日期连续性
    validateDateContinuity(days, errors, date_check_mode_);

    // 2. 逐日验证单条数据的逻辑
    for (const auto& day : days) {
        validateActivityCount(day, errors);
    }

    return errors.empty();
}

} // namespace logic
} // namespace validator