// application/steps/processed_data_writer_step.hpp
#ifndef APPLICATION_STEPS_PROCESSED_DATA_WRITER_STEP_HPP_
#define APPLICATION_STEPS_PROCESSED_DATA_WRITER_STEP_HPP_

#include "application/interfaces/i_log_serializer.hpp" // [新增]
#include "application/pipeline/interfaces/i_pipeline_step.hpp"
#include <memory>

namespace core::pipeline {

class ProcessedDataWriterStep : public IPipelineStep {
public:
  // [修改] 构造函数注�?Serializer
  explicit ProcessedDataWriterStep(
      std::shared_ptr<core::interfaces::ILogSerializer> serializer);

  bool Execute(PipelineContext &context) override;
  std::string GetName() const override { return "ProcessedDataWriter"; }

private:
  std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
};

} // namespace core::pipeline
#endif
