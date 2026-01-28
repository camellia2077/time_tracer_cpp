// infrastructure/concurrency/thread_pool_executor.cpp
#include "infrastructure/concurrency/thread_pool_executor.hpp"

namespace infrastructure::concurrency {

    ThreadPoolExecutor::ThreadPoolExecutor(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            threads_.emplace_back([this] { this->WorkerThread(); });
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

    void ThreadPoolExecutor::Submit(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            active_task_count_.fetch_add(1, std::memory_order_relaxed);
            tasks_.emplace(std::move(task));
        }
        condition_.notify_one();
    }

    void ThreadPoolExecutor::WaitAll() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        wait_condition_.wait(lock, [this]() {
            return active_task_count_.load(std::memory_order_relaxed) == 0;
        });
    }

    void ThreadPoolExecutor::WorkerThread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                
                condition_.wait(lock, [this] {
                    return stop_ || !tasks_.empty();
                });

                if (stop_ && tasks_.empty()) {
                    return;
                }

                task = std::move(tasks_.front());
                tasks_.pop();
            }

            task();

            if (active_task_count_.fetch_sub(1, std::memory_order_release) == 1) {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                wait_condition_.notify_all();
            } else {
                wait_condition_.notify_all();
            }
        }
    }

}  // namespace infrastructure::concurrency
