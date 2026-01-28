// validator/logic/rules/date_rules.cpp
#include "validator/logic/rules/date_rules.hpp"
#include <set>
#include <string>

namespace validator {
namespace logic {

namespace {
bool is_leap(int year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
int days_in_month(int year, int month) {
  if (month < 1 || month > 12)
    return 0;
  if (month == 2)
    return is_leap(year) ? 29 : 28;
  if (month == 4 || month == 6 || month == 9 || month == 11)
    return 30;
  return 31;
}
} // namespace

void validateDateContinuity(const std::vector<DailyLog> &days,
                            std::set<Error> &errors, DateCheckMode mode) {
  if (mode == DateCheckMode::None || days.empty())
    return;

  // 获取基准年月 (从第一条数据)
  const DailyLog &first_day = days.front();
  std::string first_date = first_day.date_;
  if (first_date.length() != 10)
    return;

  std::string yyyy_mm = first_date.substr(0, 7);
  int year = std::stoi(first_date.substr(0, 4));
  int month = std::stoi(first_date.substr(5, 2));

  // 收集所有已存在的日期
  std::set<int> days_found;
  for (const auto &day : days) {
    if (day.date_.rfind(yyyy_mm, 0) == 0 && day.date_.length() == 10) {
      days_found.insert(std::stoi(day.date_.substr(8, 2)));
    }
  }

  int check_until = days_in_month(year, month);
  if (mode == DateCheckMode::Continuity && !days_found.empty()) {
    check_until = *days_found.rbegin();
  }

  for (int d = 1; d <= check_until; ++d) {
    if (days_found.find(d) == days_found.end()) {
      std::string date_str =
          yyyy_mm + "-" + (d < 10 ? "0" : "") + std::to_string(d);
      errors.insert(
          {0, "Missing date: " + date_str, ErrorType::DateContinuity});
    }
  }
}

} // namespace logic
} // namespace validator
