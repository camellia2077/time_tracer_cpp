// core/application/steps/processed_data_writer_step.hpp
#ifndef CORE_APPLICATION_STEPS_PROCESSED_DATA_WRITER_STEP_HPP_
#define CORE_APPLICATION_STEPS_PROCESSED_DATA_WRITER_STEP_HPP_

#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

class ProcessedDataWriterStep : public IPipelineStep {
public:
    bool execute(PipelineContext& context) override;
    std::string get_name() const override { return "ProcessedDataWriter"; }
};

}
#endif