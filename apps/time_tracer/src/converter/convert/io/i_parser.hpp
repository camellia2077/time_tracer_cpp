// converter/convert/io/i_parser.hpp
#ifndef CONVERTER_CONVERT_IO_I_PARSER_HPP_
#define CONVERTER_CONVERT_IO_I_PARSER_HPP_

#include "core/domain/model/daily_log.hpp"
#include <iostream>
#include <functional>

/**
 * @brief 解析器接口
 * 定义了解析输入流并生成 DailyLog 对象的标准行为。
 */
class IParser {
public:
    virtual ~IParser() = default;
    
    /**
     * @brief 解析流
     * @param inputStream 输入流
     * @param onNewDay 当解析完完整的一天数据时的回调
     */
    virtual void parse(std::istream& inputStream, std::function<void(DailyLog&)> onNewDay) = 0;
};

#endif // CONVERTER_CONVERT_IO_I_PARSER_HPP_