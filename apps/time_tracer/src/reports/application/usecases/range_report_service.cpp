// reports/application/usecases/range_report_service.cpp
#include "reports/application/usecases/range_report_service.hpp"
#include "reports/data/cache/project_name_cache.hpp"
#include "reports/data/utils/project_tree_builder.hpp"
#include "reports/shared/factories/generic_formatter_factory.hpp"
#include <stdexcept>

// [新增辅助函数] (如果项目中没有现成的日期工具)
// 解析 YYYY-Www 并计算起止日期 (ISO 8601 简化版)
static std::pair<std::string, std::string>
calculate_week_range(const std::string &week_str) {
  int year = 0, week = 0;
  if (sscanf(week_str.c_str(), "%d-W%d", &year, &week) != 2)
    return {"", ""};

  std::tm tm = {};
  tm.tm_year = year - 1900;
  tm.tm_mon = 0; // Jan
  tm.tm_mday = 4;
  mktime(&tm);

  // ISO周以周一为第一天
  int wday = (tm.tm_wday == 0) ? 7 : tm.tm_wday;
  int days_shift = 1 - wday;

  tm.tm_mday += days_shift + (week - 1) * 7;
  std::time_t start_time = mktime(&tm);

  std::tm end_tm = tm;
  end_tm.tm_mday += 6;
  std::time_t end_time = mktime(&end_tm);

  char buf[16];
  std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&start_time));
  std::string s_date = buf;
  std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&end_time));
  std::string e_date = buf;

  return {s_date, e_date};
}

// [新增方法实现]
std::map<int, std::map<int, std::string>>
RangeReportService::generate_all_weekly_history(ReportFormat format) {
  std::map<int, std::map<int, std::string>> reports;

  // 1. 强制获取 Week 配置 (这修复了之前的配置加载 Bug 导致的空白问题)
  const auto &cfg = get_config_by_format(format, RangeType::Week);

  // 2. 获取底层数据 (YYYY-Www -> Stats)
  auto all_project_stats = repo_.get_all_weeks_project_stats();
  auto all_active_days = repo_.get_all_weeks_active_days();

  auto formatter =
      GenericFormatterFactory<RangeReportData>::create(format, cfg);

  for (auto &[week_str, stats] : all_project_stats) {
    RangeReportData data;
    data.report_name_ = week_str;

    // 计算日期范围
    auto [start, end] = calculate_week_range(week_str);
    data.start_date_ = start;
    data.end_date_ = end;
    data.covered_days_ = 7;

    data.project_stats_ = stats;
    data.actual_active_days_ =
        all_active_days.count(week_str) ? all_active_days[week_str] : 0;

    for (auto &p : stats)
      data.total_duration_ += p.second;

    build_project_tree_from_ids(data.project_tree_, data.project_stats_,
                                ProjectNameCache::instance());

    // 解析 Year 和 Week 用于 Map 键
    int year = 0, week = 0;
    if (sscanf(week_str.c_str(), "%d-W%d", &year, &week) == 2) {
      reports[year][week] = formatter->format_report(data);
    }
  }
  return reports;
}

static std::pair<int, int> parse_year_month(const std::string &ym) {
  if (ym.size() >= 7 && ym[4] == '-') {
    try {
      int y = std::stoi(ym.substr(0, 4));
      int m = std::stoi(ym.substr(5, 2));
      return {y, m};
    } catch (...) {
    }
  }
  return {0, 0};
}

RangeReportService::RangeReportService(IReportRepository &repo,
                                       const GlobalReportConfig &config)
    : repo_(repo), config_(config) {}

// [关键修改] 根据 type 选择 config.week / config.month / config.period
const RangeReportConfig &
RangeReportService::get_config_by_format(ReportFormat format,
                                         RangeType type) const {
  const FormatReportConfig *fmt_config = nullptr;

  switch (format) {
  case ReportFormat::Markdown:
    fmt_config = &config_.markdown_;
    break;
  case ReportFormat::LaTeX:
    fmt_config = &config_.latex_;
    break;
  case ReportFormat::Typ:
    fmt_config = &config_.typst_;
    break;
  default:
    throw std::runtime_error("Unsupported format in RangeReportService");
  }

  switch (type) {
  case RangeType::Week:
    return fmt_config->week_;
  case RangeType::Month:
    return fmt_config->month_;
  case RangeType::Period:
    return fmt_config->period_;
  default:
    return fmt_config->period_;
  }
}

std::string RangeReportService::generate_report(const RangeRequest &request,
                                                ReportFormat format) {
  // 1. 获取对应类型的配置
  const auto &cfg = get_config_by_format(format, request.type);

  // 2. 获取数据
  RangeReportData data = build_data_for_range(request);

  // 3. 格式化
  auto formatter =
      GenericFormatterFactory<RangeReportData>::create(format, cfg);
  return formatter->format_report(data);
}

// ... generate_batch 保持不变 ...
std::map<std::string, std::string>
RangeReportService::generate_batch(const std::vector<RangeRequest> &requests,
                                   ReportFormat format) {
  std::map<std::string, std::string> results;
  // 简单优化：假设批量的类型都一致，或者每次都取配置
  for (const auto &req : requests) {
    // 复用 generate_report 即可
    results[req.name] = generate_report(req, format);
  }
  return results;
}

std::map<int, std::map<int, std::string>>
RangeReportService::generate_all_monthly_history(ReportFormat format) {
  std::map<int, std::map<int, std::string>> reports;

  // 强制使用 Month 配置
  const auto &cfg = get_config_by_format(format, RangeType::Month);

  auto all_project_stats = repo_.get_all_months_project_stats();
  auto all_active_days = repo_.get_all_months_active_days();

  auto formatter =
      GenericFormatterFactory<RangeReportData>::create(format, cfg);

  for (auto &[ym, stats] : all_project_stats) {
    RangeReportData data;
    data.report_name_ = ym;
    data.start_date_ = ym + "-01";
    data.end_date_ = ym + "-31";
    data.covered_days_ = 30;
    data.project_stats_ = stats;
    data.actual_active_days_ = all_active_days[ym];

    for (auto &p : stats)
      data.total_duration_ += p.second;

    // 构建树
    build_project_tree_from_ids(data.project_tree_, data.project_stats_,
                                ProjectNameCache::instance());

    auto [y, m] = parse_year_month(ym);
    if (y > 0) {
      reports[y][m] = formatter->format_report(data);
    }
  }
  return reports;
}

RangeReportData
RangeReportService::build_data_for_range(const RangeRequest &request) {
  RangeReportData data;
  data.report_name_ = request.name;
  data.start_date_ = request.start_date;
  data.end_date_ = request.end_date;
  data.covered_days_ = request.covered_days;

  data.project_stats_ =
      repo_.get_aggregated_project_stats(request.start_date, request.end_date);
  data.actual_active_days_ =
      repo_.get_actual_active_days(request.start_date, request.end_date);

  for (const auto &[id, duration] : data.project_stats_) {
    data.total_duration_ += duration;
  }

  build_project_tree_from_ids(data.project_tree_, data.project_stats_,
                              ProjectNameCache::instance());

  return data;
}
