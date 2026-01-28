// generator/components/remark_generator.hpp
#pragma once
#ifndef GENERATOR_COMPONENTS_REMARK_GENERATOR_H_
#define GENERATOR_COMPONENTS_REMARK_GENERATOR_H_

#include "domain/model/remark_config.hpp"
#include <optional>
#include <string>
#include <random>

namespace generator {

/**
 * @class RemarkGenerator
 * @brief 专门负责生成每日备注。
 */
class RemarkGenerator {
public:
    RemarkGenerator(const std::optional<domain::model::DailyRemarkConfig>& config, std::mt19937& gen);

    std::optional<std::string> TryGenerate();

private:
    const std::optional<domain::model::DailyRemarkConfig>& remark_config_;
    std::mt19937& gen_; 

    std::optional<std::uniform_int_distribution<>> selector_;
    std::optional<std::bernoulli_distribution> should_generate_;
    std::optional<std::uniform_int_distribution<>> lines_count_dist_;
};

}  // namespace generator

#endif  // GENERATOR_COMPONENTS_REMARK_GENERATOR_H_
