// serializer/json_serializer.hpp
#ifndef SERIALIZER_JSON_SERIALIZER_HPP_
#define SERIALIZER_JSON_SERIALIZER_HPP_

#include <vector>
#include <string>
#include "common/model/daily_log.hpp"

namespace serializer {

/**
 * @brief 序列化模块的外观类 (Facade) - 基于 yyjson
 * 负责调度内部核心组件 (LogSerializer, LogDeserializer) 完成转换任务。
 * * 变更说明：
 * 原 nlohmann::json 接口已替换为 std::string。
 * 序列化方法现在返回 JSON 字符串。
 * 反序列化方法现在接收 JSON 字符串。
 */
class JsonSerializer {
public:
    // Struct -> JSON String
    static std::string serializeDay(const DailyLog& day);
    static std::string serializeDays(const std::vector<DailyLog>& days);

    // JSON String -> Struct
    static DailyLog deserializeDay(const std::string& json_content);
    static std::vector<DailyLog> deserializeDays(const std::string& json_content);
};

} // namespace serializer

#endif // SERIALIZER_JSON_SERIALIZER_HPP_