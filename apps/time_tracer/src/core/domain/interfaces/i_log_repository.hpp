// core/domain/interfaces/i_log_repository.hpp
// 日志持久化接口 - 纯业务概念
#ifndef CORE_DOMAIN_INTERFACES_I_LOG_REPOSITORY_HPP_
#define CORE_DOMAIN_INTERFACES_I_LOG_REPOSITORY_HPP_

#include <string>
#include <vector>
#include <map>
#include "core/domain/model/daily_log.hpp"

namespace core::domain::interfaces {

class ILogRepository {
public:
    virtual ~ILogRepository() = default;

    // 保存日志到持久化存储
    virtual void save(const std::map<std::string, std::vector<DailyLog>>& logs) = 0;
    
    // 按日期范围查询
    virtual std::vector<DailyLog> find_by_date(const std::string& date) = 0;
    
    // 按日期范围批量查询
    virtual std::map<std::string, std::vector<DailyLog>> find_by_date_range(
        const std::string& start_date, 
        const std::string& end_date
    ) = 0;
    
    // 检查日期是否已存在数据
    virtual bool exists(const std::string& date) = 0;
};

} // namespace core::domain::interfaces

#endif // CORE_DOMAIN_INTERFACES_I_LOG_REPOSITORY_HPP_
