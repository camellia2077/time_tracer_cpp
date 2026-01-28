// pch.hpp
#pragma once
#ifndef PCH_H
#define PCH_H

// ===================================================================
//  1. C++ 标准库 (Standard Library)
// ===================================================================
#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <memory>
#include <format>
#include <random>
#include <stdexcept>
#include <filesystem>
#include <iterator>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <map>

// ===================================================================
//  2. 平台与第三方库 (Platform & Third-Party)
// ===================================================================
#include <toml++/toml.hpp>

// ===================================================================
//  3. 项目内部头文件
// ===================================================================
#include "domain/model/config.hpp"
#include "domain/model/generation_context.hpp"
#include "domain/model/remark_config.hpp"
#include "domain/ports/file_system_interface.hpp"
#include "domain/ports/log_generator_interface.hpp"
#include "domain/ports/task_executor_interface.hpp"

#include "application/application.hpp"
#include "application/factories/log_generator_factory_interface.hpp"
#include "application/handlers/config_handler.hpp"
#include "application/handlers/workflow_handler.hpp"

#include "cli/log_generator_cli.hpp"
#include "cli/framework/ansi_colors.hpp"
#include "cli/framework/arg_parser.hpp"
#include "cli/framework/help_formatter.hpp"
#include "cli/framework/parsed_args.hpp"

#include "generator/log_generator.hpp"
#include "generator/components/day_generator.hpp"
#include "generator/components/event_generator.hpp"
#include "generator/components/remark_generator.hpp"
#include "generator/strategies/sleep_scheduler.hpp"

#include "infrastructure/concurrency/thread_pool_executor.hpp"
#include "infrastructure/factories/log_generator_factory.hpp"
#include "infrastructure/persistence/file_manager.hpp"
#include "infrastructure/persistence/toml_config_parser.hpp"

#include "utils/utils.hpp"
#include "utils/performance_reporter.hpp"

#endif //PCH_H