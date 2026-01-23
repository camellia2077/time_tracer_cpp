// validator/json/facade/json_validator.cpp
#include "validator/json/facade/json_validator.hpp"
#include <iostream>
#include "yyjson.h"
#include "validator/json/rules/date_rules.hpp"
#include "validator/json/rules/activity_rules.hpp"

namespace validator {
namespace json {

JsonValidator::JsonValidator(DateCheckMode date_check_mode)
    : date_check_mode_(date_check_mode) {}

bool JsonValidator::validate(const std::string& filename, const std::string& json_content, std::set<Error>& errors) {
    errors.clear();

    // 1. 解析 JSON (Read-only)
    // 使用 yyjson_read 解析字符串
    yyjson_doc* doc = yyjson_read(json_content.c_str(), json_content.length(), 0);
    
    if (!doc) {
        errors.insert({0, "Failed to parse JSON content in file: " + filename, ErrorType::Structural});
        return false;
    }

    // 获取根节点
    yyjson_val* root = yyjson_doc_get_root(doc);

    // 2. 检查根节点类型
    if (!yyjson_is_arr(root)) {
        errors.insert({0, "JSON root is not an array in file: " + filename, ErrorType::Structural});
        yyjson_doc_free(doc); // 记得释放
        return false;
    }

    // 3. 执行日期连续性检查 (DateRules 已更新为接收 yyjson_val*)
    validateDateContinuity(root, errors, date_check_mode_);

    // 4. 遍历每一天执行活动检查
    size_t idx, max;
    yyjson_val* day_object;
    
    yyjson_arr_foreach(root, idx, max, day_object) {
        // [假设] validateActivityCount 也已更新为接收 (yyjson_val* day_object, std::set<Error>& errors)
        validateActivityCount(day_object, errors);
    }

    // 5. 释放内存
    yyjson_doc_free(doc);

    return errors.empty();
}

} // namespace json
} // namespace validator