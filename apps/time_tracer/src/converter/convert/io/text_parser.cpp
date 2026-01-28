// converter/convert/io/text_parser.cpp
#include "converter/convert/io/text_parser.hpp"
#include "common/ansi_colors.hpp"
#include "common/utils/string_utils.hpp"
#include "common/utils/time_utils.hpp"
#include <algorithm>
#include <iostream>

// [Fix] 类型重命名
TextParser::TextParser(const LogParserConfig &config)
    : config_(config), wake_keywords_(config.wake_keywords_) {}

void TextParser::Parse(std::istream &inputStream,
                       std::function<void(DailyLog &)> onNewDay) {
  DailyLog currentDay;
  std::string line;
  std::string current_year_prefix = "";

  while (std::getline(inputStream, line)) {
    line = Trim(line);
    if (line.empty())
      continue;

    if (IsYearMarker(line)) {
      current_year_prefix = line.substr(1);
      continue;
    }

    if (current_year_prefix.empty()) {
      continue;
    }

    if (IsNewDayMarker(line)) {
      if (!currentDay.date_.empty()) {
        onNewDay(currentDay);
      }
      currentDay.Clear();
      currentDay.date_ = current_year_prefix + "-" + line.substr(0, 2) + "-" +
                         line.substr(2, 2);

    } else {
      ParseLine(line, currentDay);
    }
  }
  if (!currentDay.date_.empty()) {
    onNewDay(currentDay);
  }
}

bool TextParser::IsYearMarker(const std::string &line) const {
  if (line.length() != 5 || line[0] != 'y')
    return false;
  return std::all_of(line.begin() + 1, line.end(), ::isdigit);
}

bool TextParser::IsNewDayMarker(const std::string &line) const {
  return line.length() == 4 && std::all_of(line.begin(), line.end(), ::isdigit);
}

void TextParser::ParseLine(const std::string &line,
                           DailyLog &currentDay) const {
  const std::string &remark_prefix = config_.remark_prefix_;

  if (!remark_prefix.empty() && line.rfind(remark_prefix, 0) == 0) {
    if (!currentDay.date_.empty()) {
      currentDay.general_remarks_.push_back(
          line.substr(remark_prefix.length()));
    }
  } else if (!currentDay.date_.empty() && line.length() >= 5 &&
             std::all_of(line.begin(), line.begin() + 4, ::isdigit)) {
    std::string timeStr = line.substr(0, 4);
    std::string remaining_line = line.substr(4);

    std::string desc = Trim(remaining_line);
    std::string remark;

    size_t comment_pos = std::string::npos;
    const char *delimiters[] = {"//", "#", ";"};
    for (const char *delim : delimiters) {
      size_t pos = remaining_line.find(delim);
      if (pos != std::string::npos) {
        if (comment_pos == std::string::npos || pos < comment_pos)
          comment_pos = pos;
      }
    }
    if (comment_pos != std::string::npos) {
      desc = Trim(remaining_line.substr(0, comment_pos));
      size_t delim_len =
          (remaining_line.substr(comment_pos, 2) == "//") ? 2 : 1;
      remark = Trim(remaining_line.substr(comment_pos + delim_len));
    }

    bool is_wake = false;
    for (const auto &kw : wake_keywords_) {
      if (kw == desc) {
        is_wake = true;
        break;
      }
    }

    if (is_wake) {
      if (currentDay.getup_time_.empty()) {
        currentDay.getup_time_ = TimeUtils::FormatTime(timeStr);
      }
    } else {
      if (currentDay.getup_time_.empty() && currentDay.raw_events_.empty())
        currentDay.is_continuation_ = true;
    }
    currentDay.raw_events_.push_back({timeStr, desc, remark});
  }
}
