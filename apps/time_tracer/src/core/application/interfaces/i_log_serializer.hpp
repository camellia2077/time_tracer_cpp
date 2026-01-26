// core/application/interfaces/i_log_serializer.hpp
#ifndef CORE_APPLICATION_INTERFACES_I_LOG_SERIALIZER_HPP_
#define CORE_APPLICATION_INTERFACES_I_LOG_SERIALIZER_HPP_

#include <string>
#include <vector>
#include "core/domain/model/daily_log.hpp"

namespace core::interfaces {

/**
 * @brief 日志序列化接口
 * Core 层定义标准，Serializer 模块负责实现。
 */
class ILogSerializer {
public:
    virtual ~ILogSerializer() = default;

    /**
     * @brief 将日志列表序列化为字符串 (通常是 JSON)
     */
    virtual std::string serialize(const std::vector<DailyLog>& logs) = 0;

    /**
     * @brief 将字符串反序列化为日志列表
     */
    virtual std::vector<DailyLog> deserialize(const std::string& content) = 0;
};

} // namespace core::interfaces

#endif // CORE_APPLICATION_INTERFACES_I_LOG_SERIALIZER_HPP_