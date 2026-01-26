// config/validator/reports/facade/query_facade.cpp
#include "config/validator/reports/facade/query_facade.hpp"
#include "config/validator/reports/strategies/strategy_factory.hpp"
#include <iostream>

bool QueryFacade::validate(
    const std::vector<std::pair<std::string, toml::table>>& query_configs
) const {
    bool all_ok = true;

    for (const auto& config_pair : query_configs) {
        const std::string& file_name = config_pair.first;
        const toml::table& config_tbl = config_pair.second;

        // [修复] 使用正确的命名空间和新增的工厂方法
        auto strategy = ConfigValidator::StrategyFactory::create_from_filename(file_name);

        if (!strategy) {
            std::cerr << "[Validator] Warning: No validation strategy found for file: " << file_name << std::endl;
            continue; 
        }

        if (!strategy->validate(config_tbl, file_name)) {
            all_ok = false;
        }
    }

    if(all_ok && !query_configs.empty()) {
        std::cout << "[Validator] All query configuration files are valid." << std::endl;
    }

    return all_ok;
}