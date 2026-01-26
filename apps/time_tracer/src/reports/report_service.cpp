// reports/report_service.cpp
#include "reports/report_service.hpp"

#include "reports/services/daily_report_service.hpp"
#include "reports/services/range_report_service.hpp" // [新增] 统一的服务
#include "reports/shared/factories/generic_formatter_factory.hpp"
#include "reports/data/repositories/sqlite_report_repository.hpp"
#include "reports/data/cache/project_name_cache.hpp"
#include "reports/shared/utils/format/time_format.hpp" // 用于日期计算

// [删除] 旧的 Monthly/Period 服务引用

namespace {
    // 注册所有需要的格式化器
    void register_formatters() {
        using DayFactory = GenericFormatterFactory<DailyReportData>;
        DayFactory::register_dll_formatter(ReportFormat::Markdown, "DayMdFormatter");
        DayFactory::register_dll_formatter(ReportFormat::Typ,      "DayTypFormatter");
        DayFactory::register_dll_formatter(ReportFormat::LaTeX,    "DayTexFormatter");

        // [新增] 注册 Range 格式化器 (统一处理 Month 和 Period)
        using RangeFactory = GenericFormatterFactory<RangeReportData>;
        RangeFactory::register_dll_formatter(ReportFormat::Markdown, "RangeMdFormatter");
        RangeFactory::register_dll_formatter(ReportFormat::Typ,      "RangeTypFormatter");
        RangeFactory::register_dll_formatter(ReportFormat::LaTeX,    "RangeTexFormatter");
    }
}

ReportService::ReportService(sqlite3* db, const AppConfig& config)
    : app_config_(config) 
{
    repo_ = std::make_unique<SqliteReportRepository>(db);
    
    // 确保缓存加载 (如果需要的话，通常 Service 内部会处理，但这里预加载也没问题)
    ProjectNameCache::instance().ensure_loaded(db);
    
    register_formatters();
}

ReportService::~ReportService() = default;

// --- Single Queries ---

std::string ReportService::run_daily_query(const std::string& date, ReportFormat format) const {
    DailyReportService service(*repo_, app_config_);
    return service.generate_report(date, format);
}

std::string ReportService::run_monthly_query(const std::string& month, ReportFormat format) const {
    // [重构] 适配 RangeReportService
    RangeReportService service(*repo_, app_config_);
    
    RangeRequest request;
    request.name = month; // e.g. "2023-10"
    request.start_date = month + "-01";
    request.end_date = month + "-31"; // 字符串比较覆盖全月
    request.covered_days = 30; // 估算值，或者可以通过日期库精确计算
    
    return service.generate_report(request, format);
}

std::string ReportService::run_period_query(int days, ReportFormat format) const {
    // [重构] 适配 RangeReportService
    RangeReportService service(*repo_, app_config_);
    
    std::string today = get_current_date_str();
    
    RangeRequest request;
    request.name = "Last " + std::to_string(days) + " Days";
    request.end_date = today;
    request.start_date = add_days_to_date_str(today, -(days - 1));
    request.covered_days = days;
    
    return service.generate_report(request, format);
}

// --- Bulk Export Queries ---

FormattedGroupedReports ReportService::run_export_all_daily_reports_query(ReportFormat format) const {
    DailyReportService service(*repo_, app_config_);
    return service.generate_all_reports(format);
}

FormattedMonthlyReports ReportService::run_export_all_monthly_reports_query(ReportFormat format) const {
    // [重构] 使用 RangeReportService 的优化接口
    RangeReportService service(*repo_, app_config_);
    
    // 直接返回 Service 生成的结构，类型已匹配 (map<int, map<int, string>>)
    return service.generate_all_monthly_history(format);
}

FormattedPeriodReports ReportService::run_export_all_period_reports_query(const std::vector<int>& days_list, ReportFormat format) const {
    // [重构] 适配 RangeReportService
    RangeReportService service(*repo_, app_config_);
    std::string today = get_current_date_str();

    std::vector<RangeRequest> requests;
    requests.reserve(days_list.size());

    // 将 int 列表转换为 RangeRequest 列表
    for (int days : days_list) {
        if (days <= 0) continue;
        RangeRequest req;
        req.name = std::to_string(days); // 使用天数作为 Key，方便后续转回 int
        req.end_date = today;
        req.start_date = add_days_to_date_str(today, -(days - 1));
        req.covered_days = days;
        requests.push_back(req);
    }

    // 批量生成
    auto results_map = service.generate_batch(requests, format);

    // 转换回 FormattedPeriodReports (map<int, string>)
    FormattedPeriodReports final_reports;
    for (const auto& [name_str, content] : results_map) {
        try {
            int days = std::stoi(name_str);
            final_reports[days] = content;
        } catch (...) {
            // ignore parsing error
        }
    }

    return final_reports;
}