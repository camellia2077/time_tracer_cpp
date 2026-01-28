// validator/logic/rules/date_rules.hpp
#ifndef VALIDATOR_LOGIC_RULES_DATE_RULES_HPP_
#define VALIDATOR_LOGIC_RULES_DATE_RULES_HPP_

#include "common/types/date_check_mode.hpp"
#include "core/domain/model/daily_log.hpp"
#include "validator/common/validator_utils.hpp"
#include <set>
#include <vector>

namespace validator {
namespace logic {

void validateDateContinuity(const std::vector<DailyLog> &days,
                            std::set<Error> &errors, DateCheckMode mode);

}
} // namespace validator
#endif
