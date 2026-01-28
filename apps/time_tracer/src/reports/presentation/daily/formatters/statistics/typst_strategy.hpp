// reports/presentation/daily/formatters/statistics/typst_strategy.hpp
#ifndef REPORTS_PRESENTATION_DAILY_FORMATTERS_STATISTICS_TYPST_STRATEGY_HPP_
#define REPORTS_PRESENTATION_DAILY_FORMATTERS_STATISTICS_TYPST_STRATEGY_HPP_

#include "i_stat_strategy.hpp"
#include "reports/presentation/daily/common/day_base_config.hpp"
#include <format>
#include <string>
#include <vector>

/**
 * @brief 使用模板化配置，支持合并后的 Formatter 或独立 Config 类
 */
template <typename TConfig> class TypstStrategy : public IStatStrategy {
public:
  // 渲染期间生命周期短于 Formatter，使用原始指针即可
  explicit TypstStrategy(const TConfig *config) : config_(config) {}

  std::string format_header(const std::string &title) const override {
    std::string header;
    header += std::format("#let statistic_font_size = {}pt\n",
                          config_->GetStatisticFontSize());
    header += std::format("#let statistic_title_font_size = {}pt\n",
                          config_->GetStatisticTitleFontSize());
    header += "#set text(size: statistic_font_size)\n";
    header +=
        std::format("#text(size: statistic_title_font_size)[= {0}]\n\n", title);
    return header;
  }

  std::string format_main_item(const std::string &label,
                               const std::string &value) const override {
    return std::format("- *{0}*: {1}", label, value);
  }

  std::string format_sub_item(const std::string &label,
                              const std::string &value) const override {
    return std::format("  - *{0}*: {1}", label, value);
  }

  std::string
  build_output(const std::vector<std::string> &lines) const override {
    std::string result;
    for (const auto &line : lines) {
      result += line + "\n";
    }
    return result;
  }

private:
  const TConfig *config_;
};

#endif
