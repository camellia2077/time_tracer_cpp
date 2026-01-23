// infrastructure/concurrency/thread_pool_executor.cpp
#include "infrastructure/concurrency/thread_pool_executor.hpp"

namespace Common {

    ThreadPoolExecutor::ThreadPoolExecutor(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            threads_.emplace_back([this] { this->worker_thread(); });
        }
    }

    ThreadPoolExecutor::~ThreadPoolExecutor() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        
        for (std::thread &worker : threads_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void ThreadPoolExecutor::submit(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            // 任务加入前，计数 +1
            active_task_count_.fetch_add(1, std::memory_order_relaxed);
            tasks_.emplace(std::move(task));
        }
        condition_.notify_one();
    }

    void ThreadPoolExecutor::wait_all() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        // 等待直到所有任务完成（队列为空且无正在运行的任务）
        // 注意：这里的判断逻辑依赖 active_task_count_ 归零
        wait_condition_.wait(lock, [this]() {
            return active_task_count_.load(std::memory_order_relaxed) == 0;
        });
    }

    void ThreadPoolExecutor::worker_thread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                
                // 等待有任务或停止信号
                condition_.wait(lock, [this] {
                    return stop_ || !tasks_.empty();
                });

                if (stop_ && tasks_.empty()) {
                    return;
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }

            // 执行任务
            task();

            // 任务完成，计数 -1
            if (active_task_count_.fetch_sub(1, std::memory_order_release) == 1) {
                // 如果计数变为 0，通知 wait_all
                // 注意：这里需要再次获取锁来通知 cv 吗？通常 notify 不需要锁，
                // 但为了确保 wait_all 能看到最新的原子变量状态（虽然有 memory_order），
                // 在逻辑上通知 wait_condition_
                std::unique_lock<std::mutex> lock(queue_mutex_);
                wait_condition_.notify_all();
            } else {
                // 只是为了触发可能的检查，非必须，视具体实现而定
                // 简单的做法是直接 notify_all，让 wait_all 醒来检查 atomic
                 wait_condition_.notify_all();
            }
        }
    }

}