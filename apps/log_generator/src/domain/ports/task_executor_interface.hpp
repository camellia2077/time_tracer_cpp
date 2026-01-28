// domain/ports/task_executor_interface.hpp
// 领域层接口：任务执行器的契约定义
#pragma once
#ifndef DOMAIN_PORTS_TASK_EXECUTOR_INTERFACE_H_
#define DOMAIN_PORTS_TASK_EXECUTOR_INTERFACE_H_

#include <functional>

namespace domain::ports {

/**
 * @interface TaskExecutorInterface
 * @brief 任务执行器接口，定义并发任务提交与等待的能力
 * 
 * 这是一个技术端口接口，由 domain 层定义，infrastructure 层实现。
 * 常见实现包括线程池、异步任务调度器等。
 */
class TaskExecutorInterface {
public:
    virtual ~TaskExecutorInterface() = default;

    /**
     * @brief 提交一个无参数、无返回值的任务
     * @param task 待执行的任务
     */
    virtual void Submit(std::function<void()> task) = 0;

    /**
     * @brief 阻塞直到所有已提交的任务执行完毕
     */
    virtual void WaitAll() = 0;
};

}  // namespace domain::ports

#endif  // DOMAIN_PORTS_TASK_EXECUTOR_INTERFACE_H_
