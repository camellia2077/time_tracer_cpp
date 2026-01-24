// cli/impl/config_builder.cpp
#include "cli/impl/config_builder.hpp"
#include <stdexcept>
#include <string>

namespace Cli::Impl {

    Config ConfigBuilder::build(const Framework::ParsedArgs& args) {
        Config config;

        // [Fix] 初始化默认值，防止未定义的行为
        // 这里的默认值应与 --help 中的描述保持一致
        config.items_per_day = 10; 
        config.enable_nosleep = false;
        config.start_year = 0;
        config.end_year = 0;

        // 1. 提取基础选项
        if (auto val = args.get_value("--items")) {
            // 这里可以加简单的防御性检查，尽管 Validator 会再次检查
            config.items_per_day = std::stoi(*val);
        }

        if (args.has_flag("--nosleep")) {
            config.enable_nosleep = true;
        }

        // 2. 处理年份模式逻辑
        auto single_year = args.get_value("--year");
        auto start_year = args.get_value("--start");
        auto end_year = args.get_value("--end");

        if (single_year) {
            // 模式 A: 单一年份
            config.mode = GenerationMode::SingleYear;
            config.start_year = std::stoi(*single_year);
            config.end_year = std::stoi(*single_year);
            
            if (start_year || end_year) {
                throw std::logic_error("Cannot use --year with --start/--end.");
            }
        } else if (start_year && end_year) {
            // 模式 B: 年份范围
            config.mode = GenerationMode::YearRange;
            config.start_year = std::stoi(*start_year);
            config.end_year = std::stoi(*end_year);
        } else {
            // 错误: 参数缺失
            throw std::logic_error("Must specify --year OR (--start and --end).");
        }

        return config;
    }

}