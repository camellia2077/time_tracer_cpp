// infrastructure/concurrency/thread_pool_executor.hpp
// 基础设施层：线程池执行器
#pragma once
#ifndef INFRASTRUCTURE_CONCURRENCY_THREAD_POOL_EXECUTOR_H_
#define INFRASTRUCTURE_CONCURRENCY_THREAD_POOL_EXECUTOR_H_

#include "domain/ports/task_executor_interface.hpp"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace infrastructure::concurrency {

/**
 * @class ThreadPoolExecutor
 * @brief 线程池任务执行器实现
 * 
 * 实现 domain::ports::TaskExecutorInterface 接口，
 * 使用固定大小的线程池并行执行提交的任务。
 */
class ThreadPoolExecutor : public domain::ports::TaskExecutorInterface {
public:
    // 默认使用硬件并发数
    explicit ThreadPoolExecutor(size_t threads = std::thread::hardware_concurrency());
    ~ThreadPoolExecutor();

    // 禁止拷贝和移动
    ThreadPoolExecutor(const ThreadPoolExecutor&) = delete;
    ThreadPoolExecutor& operator=(const ThreadPoolExecutor&) = delete;

    // 实现 TaskExecutorInterface 接口
    void Submit(std::function<void()> task) override;
    void WaitAll() override;

private:
    void WorkerThread();

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    
    // 用于 WaitAll 的同步机制
    std::condition_variable wait_condition_;
    std::atomic<int> active_task_count_{0};
    
    bool stop_ = false;
};

}  // namespace infrastructure::concurrency

#endif  // INFRASTRUCTURE_CONCURRENCY_THREAD_POOL_EXECUTOR_H_