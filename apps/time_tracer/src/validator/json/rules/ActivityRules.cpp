#include "ActivityRules.hpp"
#include <string>

namespace validator {
namespace json {

void validateActivityCount(yyjson_val* day_object, std::set<Error>& errors) {
    if (!day_object || !yyjson_is_obj(day_object)) return;

    // 获取 Headers -> Date
    yyjson_val* headers = yyjson_obj_get(day_object, "headers");
    const char* date_c_str = nullptr;
    if (headers) {
        yyjson_val* date_val = yyjson_obj_get(headers, "date");
        if (date_val) date_c_str = yyjson_get_str(date_val);
    }
    std::string date_str = date_c_str ? std::string(date_c_str) : "[Unknown Date]";

    // 获取 Activities
    yyjson_val* activities = yyjson_obj_get(day_object, "activities");
    
    if (activities && yyjson_is_arr(activities)) {
        size_t count = yyjson_arr_size(activities);
        if (count < 2) {
            errors.insert({0, "In file for date " + date_str + ": The day has less than 2 activities. This may cause issues with 'sleep' activity generation.", ErrorType::Json_TooFewActivities});
        }
    } else {
        errors.insert({0, "In file for date " + date_str + ": 'activities' field is missing or not an array.", ErrorType::Json_TooFewActivities});
    }
}

} // namespace json
} // namespace validator