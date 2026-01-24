// core/application/steps/file_collector.hpp
#ifndef CORE_APPLICATION_STEPS_FILE_COLLECTOR_HPP_
#define CORE_APPLICATION_STEPS_FILE_COLLECTOR_HPP_

#include <string>
#include "core/application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

// [修改] 继承 IPipelineStep
class FileCollector : public IPipelineStep {
public:
    // [修改] 去掉 execute 的额外参数，如有需要改为构造函数传入
    bool execute(PipelineContext& context) override;
    
    std::string get_name() const override { return "FileCollector"; }
};

} 
#endif