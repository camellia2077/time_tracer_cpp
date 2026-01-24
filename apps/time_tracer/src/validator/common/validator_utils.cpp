// validator/common/validator_utils.cpp
#include "validator/common/validator_utils.hpp"
#include <sstream>
#include <vector>
#include <map>

// [移除] iostream, fstream, ansi_colors.hpp

namespace validator {

static std::string getErrorTypeHeader(ErrorType type) {
    switch (type) {
        case ErrorType::Source_RemarkAfterEvent:
        case ErrorType::Source_NoDateAtStart:
        case ErrorType::Source_InvalidLineFormat:
        case ErrorType::UnrecognizedActivity:
        case ErrorType::Source_MissingYearHeader:
            return "Source file format errors (源文件格式错误)";
        case ErrorType::IncorrectDayCountForMonth:
            return "Date Logic errors (日期逻辑错误)";
        case ErrorType::DateContinuity:
            return "Date Continuity errors (日期中断/缺失)";
        case ErrorType::TimeDiscontinuity:
            return "Activity Time Discontinuity errors (活动时间不连续)";
        case ErrorType::MissingSleepNight:
            return "Lack of sleep activity errors (最后的活动项目缺少sleep活动)";
        case ErrorType::Json_TooFewActivities:
            return "Activity count errors (活动数量错误)";
        case ErrorType::FileAccess:
            return "File access errors";
        case ErrorType::Structural:
            return "Structural errors";
        case ErrorType::LineFormat:
            return "Line format errors";
        case ErrorType::Logical:
            return "Logical consistency errors";
        default:
            return "Other errors";
    }
}

std::string format_error_report(const std::string& filename, const std::set<Error>& errors) {
    std::ostringstream oss;
    std::map<ErrorType, std::vector<Error>> grouped_errors;
    for (const auto& err : errors) {
        grouped_errors[err.type].push_back(err);
    }

    oss << "\n--------------------------------------------------\n";
    oss << "Validation Errors in File: " << filename << "\n";
    oss << "--------------------------------------------------\n";

    for (const auto& pair : grouped_errors) {
        std::string header = getErrorTypeHeader(pair.first);
        oss << "\n[" << header << "]\n";
        for (const auto& err : pair.second) {
            std::string line_info = (err.line_number == 0) ? "" : "Line " + std::to_string(err.line_number) + ": ";
            oss << "  - " << line_info << err.message << "\n";
        }
    }
    
    // 提示用户修正
    oss << "\nPlease fix the errors above manually.\n";
    
    return oss.str();
}

} // namespace validator