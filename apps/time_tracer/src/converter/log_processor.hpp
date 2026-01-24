// converter/log_processor.hpp
#ifndef CONVERTER_LOG_PROCESSOR_HPP_
#define CONVERTER_LOG_PROCESSOR_HPP_

#include <string>
#include <vector>
#include <istream> 
#include <functional>
#include <map> 
#include "core/domain/model/daily_log.hpp"
#include "common/config/models/converter_config_models.hpp"

struct LogProcessingResult {
    bool success = true;
    std::map<std::string, std::vector<DailyLog>> processed_data;
};

class LogProcessor {
public:
    explicit LogProcessor(const ConverterConfig& config);

    void convertStreamToData(std::istream& combined_stream, std::function<void(DailyLog&&)> data_consumer);

    LogProcessingResult processSourceContent(const std::string& filename, 
                                             const std::string& content);

private:
    // 这里保留总 Config，以便分发给子组件
    const ConverterConfig& config_;
};

#endif // CONVERTER_LOG_PROCESSOR_HPP_