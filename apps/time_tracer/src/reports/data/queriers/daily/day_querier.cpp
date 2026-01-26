// reports/data/queriers/daily/day_querier.cpp
#include "reports/data/queriers/daily/day_querier.hpp"
#include "reports/data/utils/project_tree_builder.hpp"
#include "reports/data/cache/project_name_cache.hpp"
#include <numeric>
#include <vector>
#include <string>

// 辅助函数：将路径部分拼接为字符串
static std::string join_path_parts(const std::vector<std::string>& parts) {
    if (parts.empty()) return "";
    std::string path = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        path += "_" + parts[i];
    }
    return path;
}

DayQuerier::DayQuerier(IReportRepository& repo, const std::string& date)
    : BaseQuerier(repo, date) {}

void DayQuerier::_prepare_data(DailyReportData& data) const {
    data.date = param_;
}

std::pair<std::string, std::string> DayQuerier::get_date_range() const {
    return {param_, param_};
}

DailyReportData DayQuerier::fetch_data() {
    // 1. 调用基类获取聚合统计
    DailyReportData data = BaseQuerier::fetch_data(); 

    // 2. 获取元数据
    data.metadata = repo_.get_day_metadata(param_);

    if (data.total_duration > 0) {
        // 3. 获取详细记录
        auto raw_records = repo_.get_time_records(param_);
        
        // 4. 将 ID 解析为完整路径
        auto& name_cache = ProjectNameCache::instance();
        // 缓存通常已在 Service 层预加载

        for (auto& record : raw_records) {
            try {
                long long pid = std::stoll(record.project_path);
                std::vector<std::string> parts = name_cache.get_path_parts(pid);
                record.project_path = join_path_parts(parts);
            } catch (...) {
                // 转换失败则保留原 ID
            }
        }
        
        data.detailed_records = std::move(raw_records);

        // 5. 获取生成统计
        data.stats = repo_.get_day_generated_stats(param_);
        
        // 6. 构建树
        build_project_tree_from_ids(data.project_tree, data.project_stats, name_cache);
    }
    return data;
}