// domain/ports/log_generator_interface.hpp
// 领域层接口：日志生成器的契约定义
#pragma once
#ifndef DOMAIN_PORTS_LOG_GENERATOR_INTERFACE_H_
#define DOMAIN_PORTS_LOG_GENERATOR_INTERFACE_H_

#include <string>

namespace domain::ports {

/**
 * @interface LogGeneratorInterface
 * @brief 日志生成器接口，定义生成日志的核心能力
 * 
 * 这是一个端口接口，由 domain 层定义，infrastructure 层实现。
 * Application 层依赖此接口而非具体实现，实现依赖倒置。
 */
class LogGeneratorInterface {
public:
    virtual ~LogGeneratorInterface() = default;

    /**
     * @brief 为指定月份生成日志内容
     * @param year 年份
     * @param month 月份
     * @param days_in_month 该月天数
     * @param buffer 输出缓冲区（引用以支持内存复用）
     */
    virtual void GenerateForMonth(int year, int month, int days_in_month, std::string& buffer) = 0;
};

}  // namespace domain::ports

#endif  // DOMAIN_PORTS_LOG_GENERATOR_INTERFACE_H_
