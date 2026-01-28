// serializer/json_serializer.cpp
#include "serializer/json_serializer.hpp"
#include "serializer/core/log_deserializer.hpp"
#include "serializer/core/log_serializer.hpp"
#include "yyjson.h"
#include <iostream>
#include <stdexcept>

namespace serializer {

using core::LogDeserializer;
using core::LogSerializer;

// --- Interface Implementations ---

std::string JsonSerializer::Serialize(const std::vector<DailyLog> &days) {
  return serializeDays(days);
}

std::vector<DailyLog> JsonSerializer::Deserialize(const std::string &content) {
  return deserializeDays(content);
}

// --- Internal Logic (Moved from static to member functions) ---

std::string JsonSerializer::serializeDay(const DailyLog &day) {
  yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
  yyjson_mut_val *root = LogSerializer::serialize(doc, day);
  yyjson_mut_doc_set_root(doc, root);

  char *json_c_str = yyjson_mut_write(doc, 0, NULL);
  std::string result = json_c_str ? std::string(json_c_str) : "{}";

  if (json_c_str)
    free(json_c_str);
  yyjson_mut_doc_free(doc);
  return result;
}

// 这里复用了原来的逻辑，只是改为了成员函数调用的形式（虽然 LogSerializer 还是
// helper）
std::string JsonSerializer::serializeDays(const std::vector<DailyLog> &days) {
  yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
  yyjson_mut_val *root_arr = yyjson_mut_arr(doc);
  yyjson_mut_doc_set_root(doc, root_arr);

  for (const auto &day : days) {
    if (!day.date_.empty()) {
      yyjson_mut_val *day_obj = LogSerializer::serialize(doc, day);
      yyjson_mut_arr_add_val(root_arr, day_obj);
    }
  }

  char *json_c_str = yyjson_mut_write(doc, 0, NULL);
  std::string result = json_c_str ? std::string(json_c_str) : "[]";

  if (json_c_str)
    free(json_c_str);
  yyjson_mut_doc_free(doc);
  return result;
}

DailyLog JsonSerializer::deserializeDay(const std::string &json_content) {
  yyjson_doc *doc = yyjson_read(json_content.c_str(), json_content.length(), 0);
  if (!doc)
    throw std::runtime_error("Failed to parse JSON string");

  try {
    yyjson_val *root = yyjson_doc_get_root(doc);
    DailyLog log = LogDeserializer::deserialize(root);
    yyjson_doc_free(doc);
    return log;
  } catch (...) {
    yyjson_doc_free(doc);
    throw;
  }
}

std::vector<DailyLog>
JsonSerializer::deserializeDays(const std::string &json_content) {
  std::vector<DailyLog> days;
  yyjson_doc *doc = yyjson_read(json_content.c_str(), json_content.length(), 0);
  if (!doc)
    return days;

  yyjson_val *root = yyjson_doc_get_root(doc);
  if (yyjson_is_arr(root)) {
    size_t idx, max;
    yyjson_val *day_val;
    yyjson_arr_foreach(root, idx, max, day_val) {
      try {
        days.push_back(LogDeserializer::deserialize(day_val));
      } catch (const std::exception &e) {
        std::cerr << "Skipping invalid day entry: " << e.what() << std::endl;
      }
    }
  }
  yyjson_doc_free(doc);
  return days;
}

} // namespace serializer
