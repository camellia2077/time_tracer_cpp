// pch.hpp
#pragma once
#ifndef PCH_HPP_
#define PCH_HPP_

// ===================================================================
//  1. C++ 标准库 (Standard Library)
//  最稳定、最庞大、使用最频繁的部分。
// ===================================================================
#include <string>                             // 使用次数: 16
#include <optional>                           // 使用次数: 11
#include <iostream>                           // 使用次数: 10
#include <vector>                             // 使用次数: 10
#include <format>                             // 使用次数: 7 (C++23)
#include <filesystem>                         // 使用次数: 5
#include <random>                             // 使用次数: 5
#include <iterator>                           // 使用次数: 4
#include <memory>                             // 使用次数: 4
#include <algorithm>                          // 使用次数: 3
#include <chrono>                             // 使用次数: 3
#include <array>                              // 使用次数: 2
#include <cmath>                              // 使用次数: 2
#include <fstream>                            // 使用次数: 2
#include <future>                             // 使用次数: 2
#include <set>                                // 使用次数: 2
#include <sstream>                            // 使用次数: 2
#include <stdexcept>                          // 使用次数: 2
#include <string_view>                        // 使用次数: 2
#include <thread>                             // 使用次数: 2
#include <map>                                // 使用次数: 1

// ===================================================================
//  2. 平台与第三方库 (Platform & Third-Party)
//  改动频率低，是 PCH 的理想候选。
// ===================================================================
#include <toml++/toml.hpp>                    // 使用次数: 2

// ===================================================================
//  3. 项目内部稳定且常用的核心头文件
//  建议仅包含极少修改的核心接口。
// ===================================================================
#include "yyjson.h"                           // 使用次数: 7

#include "cli/framework/interfaces/i_command.hpp" // 使用次数: 11
#include "cli/framework/core/command_parser.hpp" // 使用次数: 10
#include "cli/framework/core/command_registry.hpp" // 使用次数: 8
#include "cli/impl/app/app_context.hpp"       // 使用次数: 8
#include "cli/framework/core/command_validator.hpp" // 使用次数: 7
#include "cli/framework/core/arg_definitions.hpp" // 使用次数: 4
#include "cli/impl/utils/arg_utils.hpp"       // 使用次数: 4

#include "common/ansi_colors.hpp"             // 使用次数: 25
#include "common/config/app_config.hpp"       // 使用次数: 20
#include "common/model/daily_log.hpp"         // 使用次数: 17
#include "common/config/models/converter_config_models.hpp" // 使用次数: 13
#include "common/app_options.hpp"             // 使用次数: 7
#include "common/utils/string_utils.hpp"      // 使用次数: 7

#include "config/validator/reports/strategies/base_strategy.hpp" // 使用次数: 5

#include "core/pipeline/context/pipeline_context.hpp" // 使用次数: 6
#include "core/interfaces/i_workflow_handler.hpp" // 使用次数: 4

#include "io/core/file_system_helper.hpp"     // 使用次数: 8

#include "reports/shared/api/shared_api.hpp"  // 使用次数: 17
#include "reports/shared/utils/format/time_format.hpp" // 使用次数: 14
#include "reports/shared/types/report_format.hpp" // 使用次数: 12
#include "reports/data/model/daily_report_data.hpp" // 使用次数: 10
#include "reports/data/cache/project_name_cache.hpp" // 使用次数: 8
#include "reports/data/model/monthly_report_data.hpp" // 使用次数: 8
#include "reports/data/model/period_report_data.hpp" // 使用次数: 8
#include "reports/data/model/project_tree.hpp" // 使用次数: 8
#include "reports/data/utils/project_tree_builder.hpp" // 使用次数: 8
#include "reports/data/model/query_data_structs.hpp" // 使用次数: 6
#include "reports/shared/factories/generic_formatter_factory.hpp" // 使用次数: 5
#include "reports/shared/formatters/latex/tex_utils.hpp" // 使用次数: 5
#include "reports/shared/interfaces/i_report_formatter.hpp" // 使用次数: 5

#include "validator/common/ValidatorUtils.hpp" // 使用次数: 10

#endif //PCH_HPP_