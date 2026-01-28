// converter/convert/facade/converter_service.cpp
#include "converter/convert/facade/converter_service.hpp"
#include <string>

ConverterService::ConverterService(std::shared_ptr<IParser> parser,
                                   std::shared_ptr<DayProcessor> processor)
    : parser_(std::move(parser)), processor_(std::move(processor)) {}

void ConverterService::ExecuteConversion(
    std::istream &combined_input_stream,
    std::function<void(DailyLog &&)> data_consumer) {
  DailyLog previous_day;
  bool has_previous = false;

  // [DIP] 使用接口调用
  parser_->Parse(combined_input_stream, [&](DailyLog &current_day) {
    if (!has_previous) {
      DailyLog empty_day;
      processor_->Process(empty_day, current_day);
    } else {
      processor_->Process(previous_day, current_day);

      bool skip_previous = false;
      if (previous_day.date_.length() == 10 &&
          current_day.date_.length() == 10) {
        std::string prev_month = previous_day.date_.substr(5, 2);
        std::string curr_month = current_day.date_.substr(5, 2);
        if (prev_month == "12" && curr_month == "01") {
          // 简单跨年去重检测
          try {
            int prev_year = std::stoi(previous_day.date_.substr(0, 4));
            int curr_year = std::stoi(current_day.date_.substr(0, 4));
            if (curr_year == prev_year + 1)
              skip_previous = true;
          } catch (...) {
          }
        }
      }

      if (!skip_previous) {
        if (data_consumer) {
          data_consumer(std::move(previous_day));
        }
      }
    }
    previous_day = std::move(current_day);
    has_previous = true;
  });

  if (has_previous) {
    DailyLog empty_next_day;
    processor_->Process(previous_day, empty_next_day);
    if (data_consumer) {
      data_consumer(std::move(previous_day));
    }
  }
}
