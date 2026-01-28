// validator/common/validator_utils.hpp
#ifndef VALIDATOR_COMMON_VALIDATOR_UTILS_HPP_
#define VALIDATOR_COMMON_VALIDATOR_UTILS_HPP_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "common/types/date_check_mode.hpp"

namespace validator {

enum class ErrorType {
  FileAccess,
  Structural,
  LineFormat,
  TimeDiscontinuity,
  MissingSleepNight,
  Logical,
  DateContinuity,
  IncorrectDayCountForMonth,
  Source_RemarkAfterEvent,
  Source_NoDateAtStart,
  UnrecognizedActivity,
  Source_InvalidLineFormat,
  Source_MissingYearHeader,
  Json_TooFewActivities
};

struct Error {
  int line_number;
  std::string message;
  ErrorType type;

  bool operator<(const Error &other) const {
    if (line_number != other.line_number)
      return line_number < other.line_number;
    if (type != other.type)
      return type < other.type;
    return message < other.message;
  }
};

/**
 * @brief 将错误集合格式化为易读的字符串报告
 * @param filename 文件名
 * @param errors 错误集合
 * @return 格式化后的错误报告字符串
 */
std::string format_error_report(const std::string &filename,
                                const std::set<Error> &errors);

} // namespace validator

#endif // VALIDATOR_COMMON_VALIDATOR_UTILS_HPP_
