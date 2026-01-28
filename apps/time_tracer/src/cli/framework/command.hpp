// cli/framework/command.hpp
// 合并自: i_command.hpp, command_args.hpp, command_parser.hpp
#ifndef CLI_FRAMEWORK_COMMAND_HPP_
#define CLI_FRAMEWORK_COMMAND_HPP_

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

// ============================================================================
// ParserConfig - 解析器配置
// ============================================================================

/**
 * @brief 解析器配置，用于定义哪些选项是全局的，需要在过滤阶段剔除或特殊处理。
 */
struct ParserConfig {
  // 带值的选项（例如：-o, --db），解析器会跳过它们及其紧随的参数
  std::vector<std::string> global_value_options;
  // 布尔开关（例如：--verbose），解析器只会跳过它们本身
  std::vector<std::string> global_flag_options;
};

// ============================================================================
// CommandParser - 命令行解析器
// ============================================================================

class CommandParser {
public:
  explicit CommandParser(const std::vector<std::string> &args,
                         const ParserConfig &config = {});

  std::string GetCommand() const;

  // 获取过滤了全局选项后的参数列表（主要用于 Command 内部获取位置参数）
  const std::vector<std::string> &GetFilteredArgs() const;

  std::string GetRawArg(size_t index) const;

  // 获取带值的选项 (如 -o path)，基于原始参数搜索
  std::optional<std::string>
  GetOption(const std::vector<std::string> &keys) const;

  // 检查布尔开关 (如 --no-save)，基于原始参数搜索
  bool HasFlag(const std::vector<std::string> &keys) const;

private:
  std::vector<std::string> raw_args_;
  std::string command_;
  std::vector<std::string> filtered_args_;
  ParserConfig config_;

  void Parse();
  std::vector<std::string>
  FilterGlobalOptions(const std::vector<std::string> &original_args);
};

// ============================================================================
// ArgType, ArgDef - 参数定义
// ============================================================================

enum class ArgType {
  Positional, // 位置参数，如: ingest <path>
  Option,     // 带值选项，如: --format md
  Flag        // 布尔开关，如: --verbose
};

/**
 * @brief 参数定义结构
 */
struct ArgDef {
  std::string name; // 内部键名
  ArgType type;
  std::vector<std::string> keys; // 触发键 (仅 Option/Flag 有效)
  std::string help;              // 帮助文本
  bool required = false;
  std::string default_value = "";
  size_t position_index = 0; // 位置参数在 filtered_args 中的偏移量
};

// ============================================================================
// ParsedArgs - 解析后的参数结果容器
// ============================================================================

class CommandValidator; // 前向声明

/**
 * @brief 解析后的参数结果容器
 */
class ParsedArgs {
public:
  std::string Get(const std::string &name) const;
  bool Has(const std::string &name) const;
  int GetAsInt(const std::string &name) const;

private:
  std::map<std::string, std::string> values_;
  friend class CommandValidator; // 允许校验器填充数据
};

// ============================================================================
// CommandValidator - 命令行参数校验器
// ============================================================================

/**
 * @brief 命令行参数校验器
 */
class CommandValidator {
public:
  /**
   * @brief 根据定义校验并提取参数
   * @param parser 已初步解析原始字符串的解析器实例
   * @param defs 命令定义的参数列表
   */
  static ParsedArgs Validate(const CommandParser &parser,
                             const std::vector<ArgDef> &defs);
};

// ============================================================================
// ICommand - 命令接口
// ============================================================================

class ICommand {
public:
  virtual ~ICommand() = default;

  virtual std::vector<ArgDef> GetDefinitions() const = 0;

  virtual void Execute(const CommandParser &parser) = 0;

  virtual std::string GetHelp() const { return "Auto generated help..."; }
};

#endif // CLI_FRAMEWORK_COMMAND_HPP_
