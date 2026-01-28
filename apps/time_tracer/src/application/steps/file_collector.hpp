// application/steps/file_collector.hpp
#ifndef APPLICATION_STEPS_FILE_COLLECTOR_HPP_
#define APPLICATION_STEPS_FILE_COLLECTOR_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"
#include <string>

namespace core::pipeline {

// [修改] 继承 IPipelineStep
class FileCollector : public IPipelineStep {
public:
  // [修改] 去掉 execute 的额外参数，如有需要改为构造函数传?
  bool Execute(PipelineContext &context) override;

  std::string GetName() const override { return "FileCollector"; }
};

} // namespace core::pipeline
#endif
