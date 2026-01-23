// infrastructure/concurrency/i_task_executor.hpp
#ifndef INFRASTRUCTURE_CONCURRENCY_I_TASK_EXECUTOR_HPP_
#define INFRASTRUCTURE_CONCURRENCY_I_TASK_EXECUTOR_HPP_

#include <functional>

namespace Common {

    class ITaskExecutor {
    public:
        virtual ~ITaskExecutor() = default;
        
        // 提交一个无参数、无返回值的任务
        virtual void submit(std::function<void()> task) = 0;
        
        // 阻塞直到所有已提交的任务执行完毕
        virtual void wait_all() = 0;
    };

}

#endif // INFRASTRUCTURE_CONCURRENCY_I_TASK_EXECUTOR_HPP_