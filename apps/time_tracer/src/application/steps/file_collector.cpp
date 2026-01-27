// application/steps/file_collector.cpp
#include "application/steps/file_collector.hpp"
#include <algorithm>

namespace core::pipeline {

bool FileCollector::execute(PipelineContext& context) {
    const std::string extension = ".txt"; 

    if (!context.file_system->exists(context.config.input_root)) {
        context.notifier->notify_error("错误: 输入的路径不存在: " + context.config.input_root.string());
        return false;
    }

    context.state.source_files.clear();
    context.state.generated_files.clear(); 

    context.state.source_files = context.file_system->find_files(context.config.input_root, extension);

    // 检查输入本身是否就是文
    if (context.file_system->is_regular_file(context.config.input_root) && context.config.input_root.extension() == extension) {
        if (std::find(context.state.source_files.begin(), context.state.source_files.end(), context.config.input_root) == context.state.source_files.end()) {
            context.state.source_files.push_back(context.config.input_root);
        }
    }

    if (context.state.source_files.empty()) {
        context.notifier->notify_warning("警告: 在指定路径下没有找到 " + extension + " 文件");
        return false;
    }

    context.notifier->notify_info("信息: 成功收集" + std::to_string(context.state.source_files.size()) + " 个待处理文件 (" + extension + ").");
    return true;
}

}