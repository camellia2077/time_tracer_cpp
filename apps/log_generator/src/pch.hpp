// pch.hpp
#pragma once
#ifndef PCH_HPP_
#define PCH_HPP_

// ===================================================================
//  1. C++ 标准库 (Standard Library)
//  最稳定、最庞大、使用最频繁的部分。
// ===================================================================
#include <string>                             // 使用次数: 17
#include <optional>                           // 使用次数: 13
#include <iostream>                           // 使用次数: 10
#include <vector>                             // 使用次数: 10
#include <filesystem>                         // 使用次数: 6
#include <format>                             // 使用次数: 6 (C++23)
#include <memory>                             // 使用次数: 5
#include <random>                             // 使用次数: 4
#include <iterator>                           // 使用次数: 3
#include <stdexcept>                          // 使用次数: 3
#include <atomic>                             // 使用次数: 2
#include <functional>                         // 使用次数: 2
#include <mutex>                              // 使用次数: 2
#include <sstream>                            // 使用次数: 2
#include <algorithm>                          // 使用次数: 1
#include <array>                              // 使用次数: 1
#include <cmath>                              // 使用次数: 1
#include <iomanip>                            // 使用次数: 1
#include <map>                                // 使用次数: 1

// ===================================================================
//  2. 平台与第三方库 (Platform & Third-Party)
//  改动频率低，是 PCH 的理想候选。
// ===================================================================
#include <toml++/toml.hpp>                    // 使用次数: 1

// ===================================================================
//  3. 项目内部稳定且常用的核心头文件
//  建议仅包含极少修改的核心接口。
// ===================================================================
#include "cli/framework/parsed_args.hpp"      // 使用次数: 3
#include "cli/framework/arg_parser.hpp"       // 使用次数: 2
#include "cli/framework/help_formatter.hpp"   // 使用次数: 2
#include "cli/impl/config_builder.hpp"        // 使用次数: 2
#include "cli/impl/config_validator.hpp"      // 使用次数: 2
#include "cli/impl/log_generator_cli.hpp"     // 使用次数: 2

#include "common/config_types.hpp"            // 使用次数: 7
#include "common/ansi_colors.hpp"             // 使用次数: 6
#include "common/app_context.hpp"             // 使用次数: 4
#include "common/version.hpp"                 // 使用次数: 1

#include "config/config.hpp"                  // 使用次数: 5

#include "core/application.hpp"               // 使用次数: 2
#include "core/config/config_handler.hpp"     // 使用次数: 2
#include "core/workflow/workflow_handler.hpp" // 使用次数: 2

#include "generator/impl/log_generator.hpp"   // 使用次数: 3
#include "generator/api/i_log_generator.hpp"  // 使用次数: 2
#include "generator/api/log_generator_factory.hpp" // 使用次数: 2
#include "generator/components/day_generator.hpp" // 使用次数: 2
#include "generator/components/event_generator.hpp" // 使用次数: 2
#include "generator/components/remark_generator.hpp" // 使用次数: 2
#include "generator/strategies/sleep_scheduler.hpp" // 使用次数: 2
#include "generator/api/i_log_Generator.hpp"  // 使用次数: 1

#include "infrastructure/concurrency/i_task_executor.hpp" // 使用次数: 2
#include "infrastructure/concurrency/thread_pool_executor.hpp" // 使用次数: 2

#include "io/file_system_interfaces.hpp"      // 使用次数: 3
#include "io/file_manager.hpp"                // 使用次数: 2

#include "utils/utils.hpp"                    // 使用次数: 3
#include "utils/performance_reporter.hpp"     // 使用次数: 2

#endif //PCH_HPP_