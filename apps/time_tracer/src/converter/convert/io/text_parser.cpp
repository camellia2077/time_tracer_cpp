// converter/convert/io/text_parser.cpp
#include "converter/convert/io/text_parser.hpp"
#include "common/ansi_colors.hpp"
#include "common/utils/string_utils.hpp"
#include "common/utils/time_utils.hpp"
#include <algorithm>
#include <iostream>

// [Fix] 类型重命名
TextParser::TextParser(const LogParserConfig& config)
    : config_(config),
      wake_keywords_(config.wake_keywords) {}

void TextParser::parse(std::istream& inputStream, std::function<void(DailyLog&)> onNewDay) {
    DailyLog currentDay;
    std::string line;
    std::string current_year_prefix = "";

    while (std::getline(inputStream, line)) {
        line = trim(line);
        if (line.empty()) continue;

        if (isYearMarker(line)) {
            current_year_prefix = line.substr(1); 
            continue;
        }
        
        if (current_year_prefix.empty()) {
            continue;
        }

        if (isNewDayMarker(line)) {
            if (!currentDay.date.empty()) {
                onNewDay(currentDay);
            }
            currentDay.clear();
            currentDay.date = current_year_prefix + "-" + line.substr(0, 2) + "-" + line.substr(2, 2);
            
        } else {
            parseLine(line, currentDay);
        }
    }
    if (!currentDay.date.empty()) {
        onNewDay(currentDay);
    }
}

bool TextParser::isYearMarker(const std::string& line) const {
    if (line.length() != 5 || line[0] != 'y') return false;
    return std::all_of(line.begin() + 1, line.end(), ::isdigit);
}

bool TextParser::isNewDayMarker(const std::string& line) const {
    return line.length() == 4 && std::all_of(line.begin(), line.end(), ::isdigit);
}

void TextParser::parseLine(const std::string& line, DailyLog& currentDay) const {
    const std::string& remark_prefix = config_.remark_prefix;
    
    if (!remark_prefix.empty() && line.rfind(remark_prefix, 0) == 0) {
        if (!currentDay.date.empty()) {
             currentDay.generalRemarks.push_back(line.substr(remark_prefix.length()));
        }
    } else if (!currentDay.date.empty() && line.length() >= 5 && std::all_of(line.begin(), line.begin() + 4, ::isdigit)) {
        std::string timeStr = line.substr(0, 4);
        std::string remaining_line = line.substr(4);
        
        std::string desc = trim(remaining_line);
        std::string remark;
        
        size_t comment_pos = std::string::npos;
        const char* delimiters[] = {"//", "#", ";"};
        for (const char* delim : delimiters) {
            size_t pos = remaining_line.find(delim);
            if (pos != std::string::npos) {
                if (comment_pos == std::string::npos || pos < comment_pos) comment_pos = pos;
            }
        }
        if (comment_pos != std::string::npos) {
            desc = trim(remaining_line.substr(0, comment_pos));
            size_t delim_len = (remaining_line.substr(comment_pos, 2) == "//") ? 2 : 1;
            remark = trim(remaining_line.substr(comment_pos + delim_len));
        }

        bool is_wake = false;
        for(const auto& kw : wake_keywords_) {
            if (kw == desc) { is_wake = true; break; }
        }

        if (is_wake) {
            if (currentDay.getupTime.empty()) {
                currentDay.getupTime = TimeUtils::formatTime(timeStr);
            }
        } else {
            if (currentDay.getupTime.empty() && currentDay.rawEvents.empty()) currentDay.isContinuation = true;
        }
        currentDay.rawEvents.push_back({timeStr, desc, remark});
    }
}