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

LogProcessor::LogProcessor(const ConverterConfig& config) : config_(config) {}

void LogProcessor::convertStreamToData(std::istream& combined_stream, std::function<void(DailyLog&&)> data_consumer) {
    try {
        // [Composition Root] 在这里组装所有依赖
        
        // 1. 创建具体的 Parser，注入 ParserConfig
        auto parser = std::make_shared<TextParser>(config_.parser_config);
        
        // 2. 创建 Processor，注入 MapperConfig
        auto processor = std::make_shared<DayProcessor>(config_.mapper_config);

        // 3. 创建 Service，注入 Parser 接口和 Processor
        ConverterService service(parser, processor);

        // 4. 执行
        service.executeConversion(combined_stream, data_consumer);

    } catch (const std::exception& e) {
        std::cerr << RED_COLOR << "An error occurred during conversion: " << e.what() << RESET_COLOR << std::endl;
    }
}

LogProcessingResult LogProcessor::processSourceContent(const std::string& /*filename*/, const std::string& content) {
    LogProcessingResult result;
    result.success = true;
    
    try {
        std::stringstream ss(content);
        convertStreamToData(ss, [&](DailyLog&& log) {
             std::string key = log.date.substr(0, 7);
             result.processed_data[key].push_back(std::move(log));
        });
        
        // [Linker] 处理跨月连接
        // Linker 是独立步骤，注入 LinkerConfig
        LogLinker linker(config_.linker_config);
        linker.link_logs(result.processed_data);
        
    } catch (...) {
        result.success = false;
    }

    return result;
}