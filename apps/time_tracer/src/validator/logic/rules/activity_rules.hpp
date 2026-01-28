// validator/logic/rules/activity_rules.hpp
#ifndef VALIDATOR_LOGIC_RULES_ACTIVITY_RULES_HPP_
#define VALIDATOR_LOGIC_RULES_ACTIVITY_RULES_HPP_

#include "core/domain/model/daily_log.hpp"
#include "validator/common/validator_utils.hpp"
#include <set>

namespace validator {
namespace logic {

void validateActivityCount(const DailyLog &day, std::set<Error> &errors);

}
} // namespace validator
#endif
