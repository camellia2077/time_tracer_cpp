// converter/log_processor.cpp
#include "converter/log_processor.hpp"
#include "converter/convert/facade/converter_service.hpp"
#include "converter/convert/io/text_parser.hpp"
#include "converter/convert/core/day_processor.hpp"
#include "converter/convert/core/log_linker.hpp"
#include "common/ansi_colors.hpp"
#include <iostream>
#include <sstream>
#include <memory>

using core::interfaces::LogProcessingResult;

void LogProcessor::convertStreamToData(std::istream& combined_stream, 
                                     std::function<void(DailyLog&&)> data_consumer,
                                     const ConverterConfig& config) {
    try {
        // [Composition Root] 每次调用时组装依赖，使用传入的 config
        auto parser = std::make_shared<TextParser>(config.parser_config);
        auto processor = std::make_shared<DayProcessor>(config.mapper_config);

        ConverterService service(parser, processor);
        service.executeConversion(combined_stream, data_consumer);

    } catch (const std::exception& e) {
        std::cerr << RED_COLOR << "An error occurred during conversion: " << e.what() << RESET_COLOR << std::endl;
    }
}

LogProcessingResult LogProcessor::convert(const std::string& /*filename*/, 
                                        const std::string& content,
                                        const ConverterConfig& config) {
    LogProcessingResult result;
    result.success = true;
    
    try {
        std::stringstream ss(content);
        // [修改] 传递 config
        convertStreamToData(ss, [&](DailyLog&& log) {
             std::string key = log.date.substr(0, 7);
             result.processed_data[key].push_back(std::move(log));
        }, config);
        
        // [Linker] 处理跨月连接，注入 LinkerConfig
        LogLinker linker(config.linker_config);
        linker.link_logs(result.processed_data);
        
    } catch (...) {
        result.success = false;
    }

    return result;
}