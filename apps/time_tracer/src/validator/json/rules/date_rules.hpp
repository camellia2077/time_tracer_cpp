#ifndef VALIDATOR_JSON_RULES_DATE_RULES_HPP_
#define VALIDATOR_JSON_RULES_DATE_RULES_HPP_

#include "validator/common/validator_utils.hpp"
#include "yyjson.h"
#include <set>

namespace validator {
namespace json {

/**
 * @brief 验证日期连续性
 * @param days_array yyjson 数组 (包含多天数据)
 */
void validateDateContinuity(yyjson_val* days_array, std::set<Error>& errors, DateCheckMode mode);

}
}

#endif // VALIDATOR_JSON_RULES_DATE_RULES_HPP_