// utils/performance_reporter.hpp
#pragma once
#ifndef UTILS_PERFORMANCE_REPORTER_H_
#define UTILS_PERFORMANCE_REPORTER_H_

#include "domain/model/config.hpp"
#include <chrono>
#include <mutex>

namespace utils {

class PerformanceReporter {
public:
    PerformanceReporter();

    void AddGenerationTime(const std::chrono::nanoseconds& duration);
    void AddIoTime(const std::chrono::nanoseconds& duration);

    void Report(const domain::model::Config& config, int files_generated, const std::chrono::nanoseconds& total_duration) const;

private:
    std::chrono::nanoseconds total_generation_duration_;
    std::chrono::nanoseconds total_io_duration_;
    mutable std::mutex mutex_;
};

}  // namespace utils

#endif  // UTILS_PERFORMANCE_REPORTER_H_
