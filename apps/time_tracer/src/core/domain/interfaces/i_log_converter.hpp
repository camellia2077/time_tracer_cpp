// core/domain/interfaces/i_log_converter.hpp
// 日志转换接口 - 业务核心概念 (从 application/interfaces 移动)
#ifndef CORE_DOMAIN_INTERFACES_I_LOG_CONVERTER_HPP_
#define CORE_DOMAIN_INTERFACES_I_LOG_CONVERTER_HPP_

#include <string>
#include <vector>
#include <map>
#include "core/domain/model/daily_log.hpp"
#include "common/config/models/converter_config_models.hpp"

namespace core::domain::interfaces {

struct LogProcessingResult {
    bool success = true;
    std::map<std::string, std::vector<DailyLog>> processed_data;
};

class ILogConverter {
public:
    virtual ~ILogConverter() = default;

    /**
     * @brief 执行转换核心逻辑
     * @param filename 文件名（用于日志或错误报告）
     * @param content 文件内容
     * @param config 转换配置（运行时传入，实现无状态调用）
     * @return 处理结果
     */
    virtual LogProcessingResult convert(const std::string& filename, 
                                      const std::string& content, 
                                      const ConverterConfig& config) = 0;
};

} // namespace core::domain::interfaces

#endif // CORE_DOMAIN_INTERFACES_I_LOG_CONVERTER_HPP_
