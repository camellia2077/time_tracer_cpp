// validator/logic/rules/date_rules.hpp
#ifndef VALIDATOR_LOGIC_RULES_DATE_RULES_HPP_
#define VALIDATOR_LOGIC_RULES_DATE_RULES_HPP_

#include "core/domain/model/daily_log.hpp"
#include "validator/common/validator_utils.hpp"
#include "common/types/date_check_mode.hpp"
#include <vector>
#include <set>

namespace validator {
namespace logic {

void validateDateContinuity(const std::vector<DailyLog>& days, std::set<Error>& errors, DateCheckMode mode);

} 
} 
#endif