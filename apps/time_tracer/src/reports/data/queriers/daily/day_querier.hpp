// reports/data/queriers/daily/day_querier.hpp
#ifndef REPORTS_DATA_QUERIERS_DAILY_DAY_QUERIER_HPP_
#define REPORTS_DATA_QUERIERS_DAILY_DAY_QUERIER_HPP_

#include "reports/data/queriers/base_querier.hpp"
#include "reports/domain/model/daily_report_data.hpp"
#include <string>

class DayQuerier : public BaseQuerier<DailyReportData, const std::string &> {
public:
  // [修改] 构造函数接收 IReportRepository
  explicit DayQuerier(IReportRepository &repo, const std::string &date);

  DailyReportData fetch_data() override;

protected:
  // [新增] 实现基类要求的日期范围获取
  std::pair<std::string, std::string> get_date_range() const override;

  // [新增] 用于设置 report.date
  void _prepare_data(DailyReportData &data) const override;
};

#endif // REPORTS_DATA_QUERIERS_DAILY_DAY_QUERIER_HPP_
