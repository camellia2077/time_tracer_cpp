// common/config/models/converter_config_models.hpp
#ifndef COMMON_CONFIG_MODELS_CONVERTER_CONFIG_MODELS_HPP_
#define COMMON_CONFIG_MODELS_CONVERTER_CONFIG_MODELS_HPP_

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

struct DurationMappingRule {
  int less_than_minutes_ = 0;
  std::string value_;
};

// 1. 解析器专用配置 (重命名为 LogParserConfig 以避免与 CLI ParserConfig 冲突)
struct LogParserConfig {
  std::vector<std::string> wake_keywords_;
  std::string remark_prefix_;
};

// 2. 映射器专用配置
struct LogMapperConfig {
  std::vector<std::string> wake_keywords_; // 需要过滤起床事件
  std::unordered_map<std::string, std::string> text_mapping_;
  std::unordered_map<std::string, std::string> text_duration_mapping_;
  std::unordered_map<std::string, std::vector<DurationMappingRule>>
      duration_mappings_;
  std::unordered_map<std::string, std::string> top_parent_mapping_;
  std::unordered_map<std::string, std::string> initial_top_parents_;
};

// 3. 链接器专用配置
struct LogLinkerConfig {
  std::string generated_sleep_project_path_ = "sleep_night";
};

// 总配置 (聚合，用于 Loader 填充和 AppConfig 持有)
struct ConverterConfig {
  // [关键] 类型名改变，但成员变量名保持不变
  // 这样其他代码中 config.parser_config.xxx 的调用依然有效
  LogParserConfig parser_config_;
  LogMapperConfig mapper_config_;
  LogLinkerConfig linker_config_;
};

#endif // COMMON_CONFIG_MODELS_CONVERTER_CONFIG_MODELS_HPP_
