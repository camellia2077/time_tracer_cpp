// core/domain/model/query_data_structs.hpp
#ifndef CORE_DOMAIN_MODEL_QUERY_DATA_STRUCTS_HPP_
#define CORE_DOMAIN_MODEL_QUERY_DATA_STRUCTS_HPP_

#include <map>
#include <string>
#include <vector>

//  用于导出所有报告的数据结构
using FormattedGroupedReports =
    std::map<int,
             std::map<int, std::vector<std::pair<std::string, std::string>>>>;

//  用于导出所有月报的数据结构
using FormattedMonthlyReports = std::map<int, std::map<int, std::string>>;

// [新增] 用于导出所有周报的数据结构
// Map<Year, Map<Week, Content>>
using FormattedWeeklyReports = std::map<int, std::map<int, std::string>>;

// [新增] 用于导出所有年报的数据结构
// Map<Year, Content>
using FormattedYearlyReports = std::map<int, std::string>;

// 用于导出所有近期报告的数据结构
using FormattedRecentReports = std::map<int, std::string>;

#endif // CORE_DOMAIN_MODEL_QUERY_DATA_STRUCTS_HPP_
