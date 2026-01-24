// main_cli.cpp
#include <filesystem>
#include <iostream>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory> // [新增] for std::make_shared

// --- Windows-specific include for console functions ---
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

// --- 核心模块 ---
#include "bootstrap/startup_validator.hpp"
#include "common/config/app_config.hpp"
#include "config/config_loader.hpp"
#include "cli/impl/app/cli_application.hpp"

#include "io/disk_file_system.hpp"

// --- 工具与信息 ---
#include "cli/framework/interfaces/i_command.hpp"
#include "common/ansi_colors.hpp"
#include "common/version.hpp"

int main(int argc, char* argv[]) {
// --- Console Setup (Windows Only) ---
#if defined(_WIN32) || defined(_WIN64)
  SetConsoleOutputCP(CP_UTF8);
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut != INVALID_HANDLE_VALUE) {
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
    }
  }
#endif

  std::vector<std::string> args(argv, argv + argc);

  if (args.size() < 2) {
      args.push_back("--help");
  }

  bool is_help_mode = (args[1] == "-h" || args[1] == "--help");

  // 彩蛋逻辑
  if (args[1] == "tracer") {
    std::cout << std::format("\n{}{}{}\n\n", CYAN_COLOR,
                 "  \"Cheers, love! The timetracer is here.\"", RESET_COLOR);
    return 0;
  }
  if (args[1] == "motto" || args[1] == "zen") {
    std::cout << "\n";
    std::cout << std::format("{}  \"Trace your time, log your life.\"{}\n\n", CYAN_COLOR,
                 RESET_COLOR);
    return 0;
  }

  // 处理缩写命令
  if (!is_help_mode) {
      if (args[1] == "pre") args[1] = "preprocess";
      if (args[1] == "blink") args[1] = "ingest";
  }

  const std::string& command = args[1];

  if (command == "-v" || command == "--version") {
    std::cout << std::format("TimeMaster Command Version: {}\n", AppInfo::VERSION);
    std::cout << std::format("Last Updated:  {}\n", AppInfo::LAST_UPDATED);
    return 0;
  }

  // --- 启动流程：加载配置 -> 验证环境 -> 执行业务 ---
  try {
    // [新增] 0. 准备文件系统实例 (Bootstrap 阶段)
    // 这是一个独立的实例，仅用于启动时的配置加载和验证
    // CliApplication 内部会创建自己的实例用于业务逻辑
    auto bootstrap_fs = std::make_shared<io::DiskFileSystem>();

    // 1. 加载配置 (注入 fs)
    ConfigLoader boot_loader(args[0], bootstrap_fs);
    AppConfig config = boot_loader.load_configuration();

    // 2. 验证环境 (委托给 StartupValidator)
    if (!is_help_mode) {
        if (!StartupValidator::validate_environment(config)) {
          std::cerr << std::format(
                       "\n{}Configuration validation failed. Please check the "
                       "errors above.{}\n\n",
                       RED_COLOR, RESET_COLOR);
          return 1;
        }
    }

    // 3. 执行业务
    CliApplication controller(args);
    controller.execute();

  } catch (const std::exception& e) {
    std::cerr << std::format("{}Startup Error: {}{}\n", RED_COLOR, e.what(),
                 RESET_COLOR);

    if (std::string(e.what()).find("command") != std::string::npos ||
        std::string(e.what()).find("argument") != std::string::npos) {
      std::cout << std::format("\nUse '{} --help' for more information.\n", args[0]);
    }
    return 1;
  }

  return 0;
}