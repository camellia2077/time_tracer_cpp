// validator/json/facade/json_validator.cpp
#include "validator/json/facade/json_validator.hpp"
// [移除] #include <iostream>
#include "yyjson.h"
#include "validator/json/rules/date_rules.hpp"
#include "validator/json/rules/activity_rules.hpp"

namespace validator {
namespace json {

JsonValidator::JsonValidator(DateCheckMode date_check_mode)
    : date_check_mode_(date_check_mode) {}

bool JsonValidator::validate(const std::string& filename, const std::string& json_content, std::set<Error>& errors) {
    errors.clear();
    yyjson_doc* doc = yyjson_read(json_content.c_str(), json_content.length(), 0);
    
    if (!doc) {
        errors.insert({0, "Failed to parse JSON content in file: " + filename, ErrorType::Structural});
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);

    if (!yyjson_is_arr(root)) {
        errors.insert({0, "JSON root is not an array in file: " + filename, ErrorType::Structural});
        yyjson_doc_free(doc); 
        return false;
    }

    validateDateContinuity(root, errors, date_check_mode_);

    size_t idx, max;
    yyjson_val* day_object;
    yyjson_arr_foreach(root, idx, max, day_object) {
        validateActivityCount(day_object, errors);
    }

    yyjson_doc_free(doc);
    return errors.empty();
}

} // namespace json
} // namespace validator