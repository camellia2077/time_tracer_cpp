// validator/logic/rules/activity_rules.cpp
#include "validator/logic/rules/activity_rules.hpp"

namespace validator {
namespace logic {

void validateActivityCount(const DailyLog& day, std::set<Error>& errors) {
    if (day.processedActivities.size() < 2) {
        errors.insert({0, 
            "Date " + day.date + ": Has less than 2 activities. (Possible missing sleep logic)", 
            ErrorType::Json_TooFewActivities
        });
    }
}

} 
}