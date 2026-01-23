// serializer/core/log_deserializer.hpp
#ifndef SERIALIZER_CORE_LOG_DESERIALIZER_HPP_
#define SERIALIZER_CORE_LOG_DESERIALIZER_HPP_

#include "yyjson.h"
#include "common/model/daily_log.hpp"

namespace serializer::core {

class LogDeserializer {
public:
    /**
     * @brief 将 yyjson 值对象反序列化为 DailyLog 结构体
     * @throws std::runtime_error 当 JSON 结构不符合预期时
     */
    static DailyLog deserialize(yyjson_val* day_json);
};

} // namespace serializer::core

#endif // SERIALIZER_CORE_LOG_DESERIALIZER_HPP_