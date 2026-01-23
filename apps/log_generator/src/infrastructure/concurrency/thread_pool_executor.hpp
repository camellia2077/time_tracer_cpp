// infrastructure/concurrency/thread_pool_executor.hpp
#ifndef INFRASTRUCTURE_CONCURRENCY_THREAD_POOL_EXECUTOR_HPP_
#define INFRASTRUCTURE_CONCURRENCY_THREAD_POOL_EXECUTOR_HPP_

#include "infrastructure/concurrency/i_task_executor.hpp"
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace Common {

    class ThreadPoolExecutor : public ITaskExecutor {
    public:
        // 默认使用硬件并发数
        explicit ThreadPoolExecutor(size_t threads = std::thread::hardware_concurrency());
        ~ThreadPoolExecutor();

        // 禁止拷贝和移动
        ThreadPoolExecutor(const ThreadPoolExecutor&) = delete;
        ThreadPoolExecutor& operator=(const ThreadPoolExecutor&) = delete;

        // 实现 ITaskExecutor 接口
        void submit(std::function<void()> task) override;
        void wait_all() override;

    private:
        void worker_thread();

        std::vector<std::thread> threads_;
        std::queue<std::function<void()>> tasks_;
        
        std::mutex queue_mutex_;
        std::condition_variable condition_;
        
        // 用于 wait_all 的同步机制
        std::condition_variable wait_condition_;
        std::atomic<int> active_task_count_{0}; // 正在排队 + 正在执行的任务数
        
        bool stop_ = false;
    };

}

#endif // INFRASTRUCTURE_CONCURRENCY_THREAD_POOL_EXECUTOR_HPP_