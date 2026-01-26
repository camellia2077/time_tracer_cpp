// converter/convert/core/log_linker.hpp
#ifndef CONVERTER_CONVERT_CORE_LOG_LINKER_HPP_
#define CONVERTER_CONVERT_CORE_LOG_LINKER_HPP_

#include "core/domain/model/daily_log.hpp"
#include "common/config/models/converter_config_models.hpp"
#include <map>
#include <vector>
#include <string>

class LogLinker {
public:
    // [Fix] 类型重命名: LinkerConfig -> LogLinkerConfig
    explicit LogLinker(const LogLinkerConfig& config);
    void link_logs(std::map<std::string, std::vector<DailyLog>>& data_map);

private:
    const LogLinkerConfig& config_;
    void process_cross_day(DailyLog& current_day, const DailyLog& prev_day);
    void recalculate_stats(DailyLog& day);
};

#endif // CONVERTER_CONVERT_CORE_LOG_LINKER_HPP_