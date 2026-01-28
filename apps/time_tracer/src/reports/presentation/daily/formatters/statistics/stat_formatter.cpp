// reports/presentation/daily/formatters/statistics/stat_formatter.cpp
#include "reports/presentation/daily/formatters/statistics/stat_formatter.hpp"
#include "reports/presentation/daily/common/day_base_config.hpp"
#include "reports/shared/utils/report_time_format.hpp"
#include <vector>

StatFormatter::StatFormatter(std::unique_ptr<IStatStrategy> strategy)
    : m_strategy(std::move(strategy)) {
  if (!m_strategy) {
    throw std::invalid_argument(
        "Formatting strategy for StatFormatter cannot be null.");
  }
}

// [新增] 递归处理统计项的辅助函数
void process_stat_items(const std::vector<StatisticItemConfig> &items,
                        const DailyReportData &data,
                        const IStatStrategy *strategy,
                        std::vector<std::string> &lines, int level) {
  for (const auto &item : items) {
    if (!item.show_)
      continue;

    // 1. 从 data.stats 中获取数据
    long long duration = 0;
    if (!item.db_column_.empty() && data.stats_.count(item.db_column_)) {
      duration = data.stats_.at(item.db_column_);
    }

    // 2. 格式化当前行
    // 注意：目前 IStatStrategy 只定义了 main_item 和 sub_item。
    // 这里做一个简单的映射：Level 0 为 Main，Level > 0 为 Sub。
    if (level == 0) {
      // [修复] 添加第二个参数 1 (avg_days)
      lines.push_back(strategy->format_main_item(
          item.label_, time_format_duration(duration, 1)));
    } else {
      // [修复] 添加第二个参数 1 (avg_days)
      lines.push_back(strategy->format_sub_item(
          item.label_, time_format_duration(duration, 1)));
    }

    // 3. 递归处理子项
    if (!item.sub_items_.empty()) {
      process_stat_items(item.sub_items_, data, strategy, lines, level + 1);
    }
  }
}

std::string StatFormatter::format(const DailyReportData &data,
                                  const DayBaseConfig &config) const {
  const auto &items_config = config.GetStatisticsItems(); // 修改为 . 访问
  std::vector<std::string> lines_to_print;

  if (!items_config.empty()) {
    process_stat_items(items_config, data, m_strategy.get(), lines_to_print, 0);
  }

  if (lines_to_print.empty()) {
    return "";
  }

  std::string header =
      m_strategy->format_header(config.GetStatisticsLabel()); // 修改为 . 访问
  std::string body = m_strategy->build_output(lines_to_print);

  return header + body;
}
