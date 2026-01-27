// pch.hpp
#pragma once
#ifndef PCH_HPP_
#define PCH_HPP_

// ===================================================================
//  1. C++ 标准库 (Standard Library)
//  最稳定、最庞大、使用最频繁的部分。
// ===================================================================
#include <string>                             // 使用次数: 108
#include <vector>                             // 使用次数: 80
#include <memory>                             // 使用次数: 53
#include <iostream>                           // 使用次数: 46
#include <stdexcept>                          // 使用次数: 32
#include <sstream>                            // 使用次数: 29
#include <map>                                // 使用次数: 28
#include <filesystem>                         // 使用次数: 27
#include <algorithm>                          // 使用次数: 16
#include <format>                             // 使用次数: 16 (C++23)
#include <set>                                // 使用次数: 15
#include <iomanip>                            // 使用次数: 11
#include <optional>                           // 使用次数: 9
#include <functional>                         // 使用次数: 6
#include <tuple>                              // 使用次数: 5
#include <unordered_map>                      // 使用次数: 5

// ===================================================================
//  2. 平台与第三方库 (Platform & Third-Party)
//  改动频率低，是 PCH 的理想候选。
// ===================================================================
#include <sqlite3.h>                          // 使用次数: 10

#include <toml++/toml.hpp>                    // 使用次数: 30

// ===================================================================
//  3. 项目内部稳定且常用的核心头文件
//  建议仅包含极少修改的核心接口。
// ===================================================================
#include "cli/framework/core/command_parser.hpp" // 使用次数: 10
#include "cli/framework/interfaces/i_command.hpp" // 使用次数: 10
#include "cli/framework/core/command_registry.hpp" // 使用次数: 8
#include "cli/framework/core/command_validator.hpp" // 使用次数: 7
#include "cli/impl/app/app_context.hpp"       // 使用次数: 7

#include "common/ansi_colors.hpp"             // 使用次数: 12
#include "common/config/app_config.hpp"       // 使用次数: 12
#include "common/config/models/converter_config_models.hpp" // 使用次数: 12
#include "common/utils/string_utils.hpp"      // 使用次数: 8
#include "common/utils/time_utils.hpp"        // 使用次数: 8
#include "common/config/report_config_models.hpp" // 使用次数: 7
#include "common/app_options.hpp"             // 使用次数: 6
#include "common/types/date_check_mode.hpp"   // 使用次数: 6
#include "common/config/global_report_config.hpp" // 使用次数: 5

#include "config/validator/reports/strategies/base_strategy.hpp" // 使用次数: 5

#include "core/domain/model/daily_log.hpp"    // 使用次数: 20
#include "core/application/interfaces/i_file_system.hpp" // 使用次数: 13
#include "core/application/interfaces/i_log_serializer.hpp" // 使用次数: 8
#include "core/application/interfaces/i_user_notifier.hpp" // 使用次数: 8
#include "core/application/pipeline/interfaces/i_pipeline_step.hpp" // 使用次数: 8
#include "core/application/interfaces/i_log_converter.hpp" // 使用次数: 6

#include "io/disk_file_system.hpp"            // 使用次数: 5

#include "reports/shared/api/shared_api.hpp"  // 使用次数: 15
#include "reports/data/model/daily_report_data.hpp" // 使用次数: 13
#include "reports/shared/types/report_format.hpp" // 使用次数: 12
#include "reports/shared/utils/format/time_format.hpp" // 使用次数: 11
#include "reports/data/model/project_tree.hpp" // 使用次数: 8
#include "reports/data/model/range_report_data.hpp" // 使用次数: 8
#include "reports/daily/common/day_base_config.hpp" // 使用次数: 5
#include "reports/data/repositories/i_report_repository.hpp" // 使用次数: 5
#include "reports/shared/formatters/latex/tex_utils.hpp" // 使用次数: 5

#include "validator/common/validator_utils.hpp" // 使用次数: 10

#endif //PCH_HPP_