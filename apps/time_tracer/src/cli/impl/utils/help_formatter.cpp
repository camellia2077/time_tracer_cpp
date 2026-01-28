// cli/impl/utils/help_formatter.cpp
#include "cli/impl/utils/help_formatter.hpp"
#include "cli/framework/command.hpp"
#include "common/ansi_colors.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

void PrintFullUsage(
    const char *app_name,
    const std::vector<std::pair<std::string, std::unique_ptr<ICommand>>>
        &commands) {
  std::cout << kGreenColor << "TimeTracer" << kResetColor
            << ": A command-line tool for time data processing.\n\n";
  std::cout << "Usage: " << app_name
            << " <command> [arguments...] [options...]\n\n";

  // 定义分组
  struct CommandGroup {
    std::string title;
    std::vector<std::string> command_names;
  };

  std::vector<CommandGroup> groups = {
      {"Data Ingestion & Import", {"ingest", "convert", "import"}},
      {"Validation & Quality", {"validate-structure", "validate-logic"}},
      {"Statistics & Reporting", {"query", "export"}}};

  for (const auto &group : groups) {
    std::cout << kYellowColor << group.title << ":" << kResetColor << "\n";

    for (const auto &name : group.command_names) {
      // 在 commands 中查找对应的命令
      auto it = std::find_if(commands.begin(), commands.end(),
                             [&name](const auto &pair) { return pair.first == name; });

      if (it == commands.end() || !it->second)
        continue;

      const auto &cmd = it->second;
      auto defs = cmd->GetDefinitions();

      // 1. 打印命令名和简介 (对齐)
      std::string description = cmd->GetHelp();
      if (description == "Auto generated help...")
        description = "";

      std::cout << "  " << kGreenColor << std::left << std::setw(20) << name
                << kResetColor << " " << description << "\n";

      // 2. 准备用法行和详细参数
      std::vector<ArgDef> positionals;
      std::vector<ArgDef> options;
      for (const auto &def : defs) {
        if (def.type == ArgType::Positional) {
          positionals.push_back(def);
        } else {
          options.push_back(def);
        }
      }
      std::sort(positionals.begin(), positionals.end(),
                [](const ArgDef &a, const ArgDef &b) {
                  return a.position_index < b.position_index;
                });

      // 打印详细 Usage
      std::cout << "    Usage: " << name;
      for (const auto &pos : positionals) {
        std::cout << " <" << pos.name << ">";
      }
      if (!options.empty()) {
        std::cout << " [options]";
      }
      std::cout << "\n";

      // 3. 打印详细参数说明 (仅在非空时)
      if (!positionals.empty()) {
        for (const auto &def : positionals) {
          std::cout << "      " << std::left << std::setw(22)
                    << ("<" + def.name + ">") << " " << def.help
                    << (def.required ? " (Required)" : "") << "\n";
        }
      }
      if (!options.empty()) {
        for (const auto &def : options) {
          std::stringstream keys_ss;
          for (size_t i = 0; i < def.keys.size(); ++i) {
            keys_ss << def.keys[i] << (i < def.keys.size() - 1 ? ", " : "");
          }
          std::cout << "      " << std::left << std::setw(22) << keys_ss.str()
                    << " " << def.help << "\n";
        }
      }
      std::cout << "\n";
    }
  }

  std::cout << kYellowColor << "Global Options:" << kResetColor << "\n";
  std::cout << "  --help, -h          Show this help message.\n";
  std::cout << "  --version, -v       Show program version.\n";
  std::cout << "\nRun '<command> --help' for even more details on a specific "
               "command.\n";
}
