// cli/framework/parsed_args.hpp
#ifndef CLI_FRAMEWORK_PARSED_ARGS_H_
#define CLI_FRAMEWORK_PARSED_ARGS_H_

#include <string>
#include <map>
#include <optional>

namespace cli::framework {

    /**
     * @class ParsedArgs
     * @brief 解析结果容器（不可变数据对象）。
     * 仅提供查询功能，不包含解析逻辑。
     */
    class ParsedArgs {
    public:
        ParsedArgs(std::map<std::string, std::string> values, 
                   std::map<std::string, bool> flags)
            : values_(std::move(values)), flags_(std::move(flags)) {}

        bool HasFlag(const std::string& long_name) const {
            // 检查 flags 或 values 中是否存在该键
            return flags_.count(long_name) > 0 || values_.count(long_name) > 0;
        }

        std::optional<std::string> GetValue(const std::string& long_name) const {
            auto it = values_.find(long_name);
            if (it != values_.end()) {
                return it->second;
            }
            return std::nullopt;
        }

    private:
        std::map<std::string, std::string> values_;
        std::map<std::string, bool> flags_;
    };

}  // namespace cli::framework

#endif // CLI_FRAMEWORK_PARSED_ARGS_H_