// validator/json/rules/activity_rules.hpp
#ifndef VALIDATOR_JSON_RULES_ACTIVITY_RULES_HPP_
#define VALIDATOR_JSON_RULES_ACTIVITY_RULES_HPP_

#include "validator/common/validator_utils.hpp"
#include "yyjson.h"
#include <set>

namespace validator {
namespace json {

/**
 * @brief 验证一天的活动数量是否达标
 * @param day_object yyjson 对象 (代表一天的数据)
 * @param errors 错误集合
 */
void validateActivityCount(yyjson_val* day_object, std::set<Error>& errors);

} // namespace json
} // namespace validator

#endif // VALIDATOR_JSON_RULES_ACTIVITY_RULES_HPP_