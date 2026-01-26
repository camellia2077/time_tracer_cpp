// core/application/steps/converter_step.cpp
#include "core/application/steps/converter_step.hpp"
// [移除] #include "converter/log_processor.hpp" -> 现在通过接口调用，不需要具体头文件
#include <chrono>
#include <iomanip>
#include <future> 
#include <vector>
#include <mutex>
#include <sstream>

namespace core::pipeline {

// [修改] 注入 converter
ConverterStep::ConverterStep(std::shared_ptr<core::interfaces::ILogConverter> converter) 
    : converter_(std::move(converter)) {}

bool ConverterStep::execute(PipelineContext& context) {
    context.notifier->notify_info("Step: Converting files (Parallel)...");
    auto start_time = std::chrono::steady_clock::now();

    std::mutex data_mutex;
    using core::interfaces::LogProcessingResult; // 使用接口中定义的 Result
    std::vector<std::future<LogProcessingResult>> futures;
    
    // 捕获 converter_ 指针
    auto converter = converter_;
    // 捕获 config 副本 (ConverterConfig 是轻量级数据结构，或者引用 context 也可以)
    auto config = context.state.converter_config;

    for (const auto& file_path : context.state.source_files) {
        futures.push_back(std::async(std::launch::async, [context_fs = context.file_system, converter, config, file_path]() -> LogProcessingResult {
            try {
                std::string content = context_fs->read_content(file_path);
                // [关键修改] 调用接口方法，传入 filename, content 和 config
                return converter->convert(file_path.string(), content, config);
            } catch (const std::exception&) {
                 return LogProcessingResult{false, {}}; 
            }
        }));
    }

    bool all_success = true;
    int processed_count = 0;

    for (size_t i = 0; i < futures.size(); ++i) {
        LogProcessingResult result = futures[i].get(); 
        processed_count++;

        if (!result.success) {
            all_success = false;
            context.notifier->notify_error("转换失败: " + context.state.source_files[i].filename().string());
        } else {
            std::lock_guard<std::mutex> lock(data_mutex);
            context.result.processed_data.insert(result.processed_data.begin(), result.processed_data.end());
        }
    }
    
    auto end_time = std::chrono::steady_clock::now();
    double duration = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    printTiming(duration, context.notifier);
    
    if (all_success) {
        context.notifier->notify_success("内存转换阶段 全部成功 (" + std::to_string(processed_count) + " files).");
    } else {
        context.notifier->notify_warning("内存转换阶段 完成，但存在部分错误。");
    }

    return all_success;
}

void ConverterStep::printTiming(double total_time_ms, const std::shared_ptr<core::interfaces::IUserNotifier>& notifier) const {
    double total_time_s = total_time_ms / 1000.0;
    std::ostringstream oss;
    oss << "转换耗时: " << std::fixed << std::setprecision(3) << total_time_s << " 秒 (" << total_time_ms << " ms)";
    notifier->notify_info(oss.str());
}

}