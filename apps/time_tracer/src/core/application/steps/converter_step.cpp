// core/application/steps/converter_step.cpp
#include "core/application/steps/converter_step.hpp"
#include "converter/log_processor.hpp"
#include <chrono>
#include <iomanip>
#include <future> 
#include <vector>
#include <mutex>
#include <sstream>

namespace core::pipeline {

ConverterStep::ConverterStep(const AppConfig& /*config*/) {}

bool ConverterStep::execute(PipelineContext& context) {
    context.notifier->notify_info("Step: Converting files (Parallel)...");
    auto start_time = std::chrono::steady_clock::now();

    std::mutex data_mutex;
    std::vector<std::future<LogProcessingResult>> futures;
    
    // 注意：在线程中调用 notifier 需要确保 notifier 实现是线程安全的，
    // 或者我们只在主线程汇总输出。这里选择在主线程输出错误，或者简单地捕获异常信息。
    // 为了简单起见，我们在 lambda 中只做计算，不直接调用 context.notifier (防止竞争)
    
    for (const auto& file_path : context.state.source_files) {
        futures.push_back(std::async(std::launch::async, [&context, file_path]() -> LogProcessingResult {
            try {
                std::string content = context.file_system->read_content(file_path);
                LogProcessor processor(context.state.converter_config);
                return processor.processSourceContent(file_path.string(), content);
            } catch (const std::exception& e) {
                 // 线程内无法安全访问 UI，将错误通过 Result 返回或 log
                 // 这里简单返回失败
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
            // 可以在这里补充输出具体的失败文件（如果 Result 包含文件名）
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

// 修改 printTiming 接收 notifier
void ConverterStep::printTiming(double total_time_ms, const std::shared_ptr<core::interfaces::IUserNotifier>& notifier) const {
    double total_time_s = total_time_ms / 1000.0;
    std::ostringstream oss;
    oss << "转换耗时: " << std::fixed << std::setprecision(3) << total_time_s << " 秒 (" << total_time_ms << " ms)";
    notifier->notify_info(oss.str());
}

}