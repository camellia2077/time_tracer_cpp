// serializer/json_serializer.cpp
#include "json_serializer.hpp"
#include "serializer/core/log_deserializer.hpp"
#include "serializer/core/log_serializer.hpp"
#include "yyjson.h"
#include <iostream>
#include <stdexcept>

namespace serializer {

using core::LogSerializer;
using core::LogDeserializer;

// --- Serialization Facade ---

std::string JsonSerializer::serializeDay(const DailyLog& day) {
    // 1. 创建文档
    yyjson_mut_doc* doc = yyjson_mut_doc_new(NULL);
    
    // 2. 构建根对象
    yyjson_mut_val* root = LogSerializer::serialize(doc, day);
    yyjson_mut_doc_set_root(doc, root);
    
    // 3. 导出为字符串
    char* json_c_str = yyjson_mut_write(doc, 0, NULL);
    std::string result = json_c_str ? std::string(json_c_str) : "{}";
    
    // 4. 清理内存
    if (json_c_str) free(json_c_str);
    yyjson_mut_doc_free(doc);
    
    return result;
}

std::string JsonSerializer::serializeDays(const std::vector<DailyLog>& days) {
    // 1. 创建文档
    yyjson_mut_doc* doc = yyjson_mut_doc_new(NULL);
    
    // 2. 构建根数组
    yyjson_mut_val* root_arr = yyjson_mut_arr(doc);
    yyjson_mut_doc_set_root(doc, root_arr);

    for (const auto& day : days) {
        if (!day.date.empty()) {
            yyjson_mut_val* day_obj = LogSerializer::serialize(doc, day);
            yyjson_mut_arr_add_val(root_arr, day_obj);
        }
    }

    // 3. 导出为字符串
    char* json_c_str = yyjson_mut_write(doc, 0, NULL);
    std::string result = json_c_str ? std::string(json_c_str) : "[]";
    
    // 4. 清理内存
    if (json_c_str) free(json_c_str);
    yyjson_mut_doc_free(doc);
    
    return result;
}

// --- Deserialization Facade ---

DailyLog JsonSerializer::deserializeDay(const std::string& json_content) {
    // 1. 读取文档 (Immutable)
    yyjson_doc* doc = yyjson_read(json_content.c_str(), json_content.length(), 0);
    if (!doc) {
        throw std::runtime_error("Failed to parse JSON string");
    }

    try {
        yyjson_val* root = yyjson_doc_get_root(doc);
        DailyLog log = LogDeserializer::deserialize(root);
        yyjson_doc_free(doc);
        return log;
    } catch (...) {
        yyjson_doc_free(doc);
        throw;
    }
}

std::vector<DailyLog> JsonSerializer::deserializeDays(const std::string& json_content) {
    std::vector<DailyLog> days;
    
    // 1. 读取文档
    yyjson_doc* doc = yyjson_read(json_content.c_str(), json_content.length(), 0);
    if (!doc) {
         // 解析失败返回空列表或抛出异常，这里维持原逻辑返回空
        return days;
    }

    yyjson_val* root = yyjson_doc_get_root(doc);
    if (yyjson_is_arr(root)) {
        size_t idx, max;
        yyjson_val* day_val;
        yyjson_arr_foreach(root, idx, max, day_val) {
            try {
                days.push_back(LogDeserializer::deserialize(day_val));
            } catch (const std::exception& e) {
                std::cerr << "Skipping invalid day entry: " << e.what() << std::endl;
            }
        }
    }
    
    yyjson_doc_free(doc);
    return days;
}

} // namespace serializer