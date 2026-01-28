// config/validators/report_validator.hpp
// 合并自: validator/reports/strategies/*.hpp
#ifndef CONFIG_VALIDATORS_REPORT_VALIDATOR_HPP_
#define CONFIG_VALIDATORS_REPORT_VALIDATOR_HPP_

#include "core/domain/types/report_format.hpp"
#include <memory>
#include <string>
#include <toml++/toml.hpp>

namespace ConfigValidator {

// ============================================================================
// IQueryStrategy - 查询配置验证策略接口
// ============================================================================
class IQueryStrategy {
public:
  virtual ~IQueryStrategy() = default;
  virtual bool validate(const toml::table &query_config,
                        const std::string &file_name) const = 0;
};

// ============================================================================
// BaseStrategy - 策略基类
// ============================================================================
class BaseStrategy : public IQueryStrategy {
public:
  bool validate(const toml::table &query_config,
                const std::string &file_name) const final;
  virtual ~BaseStrategy() = default;

protected:
  virtual bool validate_specific_keys(const toml::table &query_config,
                                      const std::string &file_name) const = 0;

private:
  bool validate_common_rules(const toml::table &query_config,
                             const std::string &file_name) const;
  bool validate_statistics_items(const toml::table &query_config,
                                 const std::string &file_name) const;
  bool is_valid_hex_color(const std::string &color_string) const;
};

// ============================================================================
// Daily Strategies
// ============================================================================
class DailyTexStrategy : public BaseStrategy {
protected:
  bool validate_specific_keys(const toml::table &query_config,
                              const std::string &file_name) const override;
};

class DailyTypStrategy : public BaseStrategy {
protected:
  bool validate_specific_keys(const toml::table &query_config,
                              const std::string &file_name) const override;
};

class DailyMdStrategy : public BaseStrategy {
protected:
  bool validate_specific_keys(const toml::table &query_config,
                              const std::string &file_name) const override;
};

// ============================================================================
// Range Strategy (Month, Week, Recent)
// ============================================================================
class RangeStrategy : public BaseStrategy {
protected:
  bool validate_specific_keys(const toml::table &query_config,
                              const std::string &file_name) const override;
};

// ============================================================================
// StrategyFactory - 策略工厂
// ============================================================================
enum class ReportType { Daily, Monthly, Weekly, Recent, Range };

class StrategyFactory {
public:
  static std::unique_ptr<IQueryStrategy> create(ReportType type,
                                                ReportFormat format);
  static std::unique_ptr<IQueryStrategy>
  create_from_filename(const std::string &file_name);
};

} // namespace ConfigValidator

#endif // CONFIG_VALIDATORS_REPORT_VALIDATOR_HPP_
