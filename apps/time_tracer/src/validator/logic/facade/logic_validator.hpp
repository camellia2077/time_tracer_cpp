// validator/logic/facade/logic_validator.hpp
#ifndef VALIDATOR_LOGIC_FACADE_LOGIC_VALIDATOR_HPP_
#define VALIDATOR_LOGIC_FACADE_LOGIC_VALIDATOR_HPP_

#include "common/types/date_check_mode.hpp"
#include "core/domain/model/daily_log.hpp"
#include "validator/common/validator_utils.hpp"
#include <set>
#include <string>
#include <vector>

namespace validator {
namespace logic {

/**
 * @brief 业务逻辑验证器
 * 替代原有的 JsonValidator，直接对领域对象进行逻辑检查。
 */
class LogicValidator {
public:
  explicit LogicValidator(DateCheckMode date_check_mode = DateCheckMode::None);

  /**
   * @brief 执行验证
   * @param context_name 上下文名称（通常是月份，如 "2025-01"，用于日志标识）
   * @param days 待验证的日报列表
   * @param errors 错误集合输出
   * @return 是否验证通过 (errors 为空时返回 true)
   */
  bool validate(const std::string &context_name,
                const std::vector<DailyLog> &days, std::set<Error> &errors);

private:
  DateCheckMode date_check_mode_;
};

} // namespace logic
} // namespace validator

#endif // VALIDATOR_LOGIC_FACADE_LOGIC_VALIDATOR_HPP_
