#ifndef CONVERTER_CONVERT_IO_TEXT_PARSER_HPP_
#define CONVERTER_CONVERT_IO_TEXT_PARSER_HPP_

#include "converter/convert/io/i_parser.hpp"
#include "common/config/models/converter_config_models.hpp"
#include <vector>

class TextParser : public IParser {
public:
    // [Fix] 类型重命名: ParserConfig -> LogParserConfig
    explicit TextParser(const LogParserConfig& config);
    void parse(std::istream& inputStream, std::function<void(DailyLog&)> onNewDay) override;

private:
    const LogParserConfig& config_;
    
    // 缓存引用
    const std::vector<std::string>& wake_keywords_;

    bool isYearMarker(const std::string& line) const;
    bool isNewDayMarker(const std::string& line) const;
    void parseLine(const std::string& line, DailyLog& currentDay) const;
};

#endif // CONVERTER_CONVERT_IO_TEXT_PARSER_HPP_