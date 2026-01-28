// reports/shared/generators/base_generator.hpp
#ifndef REPORTS_SHARED_GENERATORS_BASE_GENERATOR_HPP_
#define REPORTS_SHARED_GENERATORS_BASE_GENERATOR_HPP_

#include "common/config/app_config.hpp"
#include "core/domain/types/report_format.hpp"
#include "reports/shared/factories/generic_formatter_factory.hpp"
#include <memory>
#include <string>
// [修改] 引入 Repository 接口
#include "reports/domain/repositories/i_report_repository.hpp"

/**
 * @class BaseGenerator
 * @brief (模板基类) 为所有报告生成器提供通用的报告生成流程。
 *
 * @tparam ReportDataType 报告数据类型
 * @tparam QuerierType 查询器类型 (必须接受 IReportRepository& 和
 * QueryParamType)
 * @tparam QueryParamType 查询参数类型
 */
template <typename ReportDataType, typename QuerierType,
          typename QueryParamType>
class BaseGenerator {
public:
  /**
   * @brief 构造函数。
   * @param repo 数据仓储接口的引用。
   * @param config 应用程序配置。
   */
  // [修改] 接收 IReportRepository& 替代 sqlite3*
  BaseGenerator(IReportRepository &repo, const AppConfig &config)
      : repo_(repo), app_config_(config) {}

  virtual ~BaseGenerator() = default;

  std::string generate_report(QueryParamType param, ReportFormat format) const {
    // 1. 创建具体的查询器并获取数据 (传入 Repository)
    QuerierType querier(repo_, param);
    ReportDataType report_data = querier.fetch_data();

    // 2. 使用通用工厂创建格式化器
    auto formatter =
        GenericFormatterFactory<ReportDataType>::create(format, app_config_);

    // 3. 格式化并返回报告
    return formatter->format_report(report_data);
  }

protected:
  // [修改] 持有 Repository 引用
  IReportRepository &repo_;
  const AppConfig &app_config_;
};

#endif // REPORTS_SHARED_GENERATORS_BASE_GENERATOR_HPP_
