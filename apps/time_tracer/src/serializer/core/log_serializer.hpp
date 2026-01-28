// serializer/core/log_serializer.hpp
#ifndef SERIALIZER_CORE_LOG_SERIALIZER_HPP_
#define SERIALIZER_CORE_LOG_SERIALIZER_HPP_

#include "core/domain/model/daily_log.hpp"
#include "yyjson.h"

namespace serializer::core {

class LogSerializer {
public:
  /**
   * @brief 将 DailyLog 序列化为 yyjson 可变对象
   * @param doc 所属的 yyjson 文档（用于内存分配）
   * @param day 数据源
   * @return 指向生成的 yyjson 对象的指针
   */
  static yyjson_mut_val *serialize(yyjson_mut_doc *doc, const DailyLog &day);
};

} // namespace serializer::core

#endif // SERIALIZER_CORE_LOG_SERIALIZER_HPP_
