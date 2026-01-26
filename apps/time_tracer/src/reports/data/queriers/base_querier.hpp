// reports/data/queriers/base_querier.hpp
#ifndef REPORTS_DATA_QUERIERS_BASE_QUERIER_HPP_
#define REPORTS_DATA_QUERIERS_BASE_QUERIER_HPP_

#include "reports/data/repositories/i_report_repository.hpp"
#include <string>
#include <vector>
#include <stdexcept>

template<typename ReportDataType, typename QueryParamType>
class BaseQuerier {
public:
    // [修改] 注入 Repository 接口
    explicit BaseQuerier(IReportRepository& repo, QueryParamType param)
        : repo_(repo), param_(param) {}

    virtual ~BaseQuerier() = default;

    virtual ReportDataType fetch_data() {
        ReportDataType data;
        if (!_validate_input()) {
            _handle_invalid_input(data);
            return data;
        }
        
        _prepare_data(data);
        
        // [修改] 直接调用 Repository 方法
        // 这里需要子类提供具体的日期范围，或者我们在 BaseQuerier 中计算好 range
        auto [start, end] = get_date_range();
        data.project_stats = repo_.get_aggregated_project_stats(start, end);

        // 计算总时长
        long long total = 0;
        for(auto& p : data.project_stats) total += p.second;
        data.total_duration = total;

        return data;
    }

protected:
    IReportRepository& repo_;
    QueryParamType param_;

    // [新增] 纯虚函数：获取查询的日期范围
    virtual std::pair<std::string, std::string> get_date_range() const = 0;

    virtual bool _validate_input() const { return true; }
    virtual void _handle_invalid_input(ReportDataType& /*data*/) const {}
    virtual void _prepare_data(ReportDataType& /*data*/) const {}
    
    // 移除 SQL 相关的虚函数: get_date_condition_sql, bind_sql_parameters
    // 移除 _fetch_records_and_duration
};

#endif