// converter/log_processor.hpp
#ifndef CONVERTER_LOG_PROCESSOR_HPP_
#define CONVERTER_LOG_PROCESSOR_HPP_

#include "application/interfaces/i_log_converter.hpp" // [新增] 实现接口
#include <istream> 
#include <functional>

// [移除] struct LogProcessingResult 定义，已移动到接口文件中

class LogProcessor : public core::interfaces::ILogConverter {
public:
    LogProcessor() = default; // [修改] 默认构造，不再持有 config

    // --- 实现接口 ---
    core::interfaces::LogProcessingResult convert(const std::string& filename, 
                                                const std::string& content, 
                                                const ConverterConfig& config) override;

private:
    // 内部辅助方法，也需要传�?config
    void convertStreamToData(std::istream& combined_stream, 
                             std::function<void(DailyLog&&)> data_consumer,
                             const ConverterConfig& config);
};

#endif // CONVERTER_LOG_PROCESSOR_HPP_