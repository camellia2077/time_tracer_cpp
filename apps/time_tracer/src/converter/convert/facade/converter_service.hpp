// converter/convert/facade/converter_service.hpp
#ifndef CONVERTER_CONVERT_FACADE_CONVERTER_SERVICE_HPP_
#define CONVERTER_CONVERT_FACADE_CONVERTER_SERVICE_HPP_

#include "converter/convert/core/day_processor.hpp" // Processor 暂时保留具体类，也可提取接口
#include "converter/convert/io/i_parser.hpp" // [DIP] 依赖接口
#include "core/domain/model/daily_log.hpp"
#include <functional>
#include <istream>
#include <memory>

class ConverterService {
public:
  // [DIP] 构造函数注入：不再自己 `new` Parser，而是由外部传入
  ConverterService(std::shared_ptr<IParser> parser,
                   std::shared_ptr<DayProcessor> processor);

  void ExecuteConversion(std::istream &combined_input_stream,
                         std::function<void(DailyLog &&)> data_consumer);

private:
  std::shared_ptr<IParser> parser_;
  std::shared_ptr<DayProcessor> processor_;
};

#endif // CONVERTER_CONVERT_FACADE_CONVERTER_SERVICE_HPP_
