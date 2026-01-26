// config/validator/reports/strategies/strategy_factory.cpp
#include "config/validator/reports/strategies/strategy_factory.hpp"
#include "config/validator/reports/strategies/daily/daily_md.hpp"
#include "config/validator/reports/strategies/daily/daily_tex.hpp"
#include "config/validator/reports/strategies/daily/daily_typ.hpp"
#include "config/validator/reports/strategies/range/range_strategy.hpp"
#include <stdexcept>

namespace ConfigValidator {

std::unique_ptr<IQueryStrategy> StrategyFactory::create(ReportType type, ReportFormat format) {
    switch (type) {
        case ReportType::Daily:
            switch (format) {
                case ReportFormat::Markdown: return std::make_unique<DailyMdStrategy>();
                case ReportFormat::LaTeX:    return std::make_unique<DailyTexStrategy>();
                case ReportFormat::Typ:      return std::make_unique<DailyTypStrategy>();
                default: throw std::invalid_argument("Unsupported format for Daily report");
            }
        case ReportType::Monthly:
        case ReportType::Periodic:
            return std::make_unique<RangeStrategy>();
        default:
            throw std::invalid_argument("Unsupported report type");
    }
}

// [新增实现] 逻辑解析文件名
std::unique_ptr<IQueryStrategy> StrategyFactory::create_from_filename(const std::string& file_name) {
    ReportType type = ReportType::Daily;
    if (file_name.find("Month") != std::string::npos) type = ReportType::Monthly;
    else if (file_name.find("Period") != std::string::npos) type = ReportType::Periodic;

    ReportFormat format = ReportFormat::Markdown;
    if (file_name.find("Tex") != std::string::npos) format = ReportFormat::LaTeX;
    else if (file_name.find("Typ") != std::string::npos) format = ReportFormat::Typ;

    return create(type, format);
}

} // namespace ConfigValidator