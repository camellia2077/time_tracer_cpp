// application/steps/file_collector.cpp
#include "application/steps/file_collector.hpp"
#include <algorithm>

namespace core::pipeline {

bool FileCollector::Execute(PipelineContext &context) {
  context.notifier->NotifyInfo("Step: File Collector");
  const std::string extension = ".txt";

  if (!context.file_system->Exists(context.config.input_root_)) {
    context.notifier->NotifyError("错误: 输入的路径不存在: " +
                                  context.config.input_root_.string());
    return false;
  }

  context.state.source_files.clear();
  context.state.generated_files.clear();

  context.state.source_files =
      context.file_system->FindFiles(context.config.input_root_, extension);

  // 检查输入本身是否就是文件
  if (context.file_system->IsRegularFile(context.config.input_root_) &&
      context.config.input_root_.extension() == extension) {
    if (std::find(context.state.source_files.begin(),
                  context.state.source_files.end(),
                  context.config.input_root_) ==
        context.state.source_files.end()) {
      context.state.source_files.push_back(context.config.input_root_);
    }
  }

  if (context.state.source_files.empty()) {
    context.notifier->NotifyWarning("警告: 在指定路径下没有找到 " + extension +
                                    " 文件");
    return false;
  }

  context.notifier->NotifyInfo(
      "信息: 成功收集" + std::to_string(context.state.source_files.size()) +
      " 个待处理文件 (" + extension + ").");
  return true;
}

} // namespace core::pipeline
