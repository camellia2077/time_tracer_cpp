// validator/logic/rules/activity_rules.cpp
#include "validator/logic/rules/activity_rules.hpp"

namespace validator {
namespace logic {

void validateActivityCount(const DailyLog &day, std::set<Error> &errors) {
  if (day.processed_activities_.size() < 2) {
    errors.insert(
        {0,
         "Date " + day.date_ +
             ": Has less than 2 activities. (Possible missing sleep logic)",
         ErrorType::Json_TooFewActivities});
  }
}

} // namespace logic
} // namespace validator
