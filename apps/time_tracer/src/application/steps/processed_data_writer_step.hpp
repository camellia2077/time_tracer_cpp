// application/steps/processed_data_writer_step.hpp
#ifndef APPLICATION_STEPS_PROCESSED_DATA_WRITER_STEP_HPP_
#define APPLICATION_STEPS_PROCESSED_DATA_WRITER_STEP_HPP_

#include "application/pipeline/interfaces/i_pipeline_step.hpp"
#include "application/interfaces/i_log_serializer.hpp" // [新增]
#include <memory>

namespace core::pipeline {

class ProcessedDataWriterStep : public IPipelineStep {
public:
    // [修改] 构造函数注�?Serializer
    explicit ProcessedDataWriterStep(std::shared_ptr<core::interfaces::ILogSerializer> serializer);

    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "ProcessedDataWriter"; }

private:
    std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
};

}
#endif