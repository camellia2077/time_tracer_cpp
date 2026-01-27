// application/steps/file_collector.hpp
#ifndef APPLICATION_STEPS_FILE_COLLECTOR_HPP_
#define APPLICATION_STEPS_FILE_COLLECTOR_HPP_

#include <string>
#include "application/pipeline/interfaces/i_pipeline_step.hpp"

namespace core::pipeline {

// [修改] 继承 IPipelineStep
class FileCollector : public IPipelineStep {
public:
    // [修改] 去掉 execute 的额外参数，如有需要改为构造函数传�?
    bool execute(PipelineContext& context) override;
    
    std::string get_name() const override { return "FileCollector"; }
};

} 
#endif