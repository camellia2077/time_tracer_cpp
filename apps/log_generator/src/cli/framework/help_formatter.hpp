// cli/framework/help_formatter.hpp
#ifndef CLI_FRAMEWORK_HELP_FORMATTER_H_
#define CLI_FRAMEWORK_HELP_FORMATTER_H_

#include <string>
#include <vector>

namespace cli::framework {

    struct Option {
        std::string short_name;
        std::string long_name;
        std::string description;
        bool requires_value = false;
    };

    /**
     * @class HelpFormatter
     * @brief 负责命令行工具的输出格式化。
     */
    class HelpFormatter {
    public:
        explicit HelpFormatter(std::string prog_name);

        void PrintHelp(const std::vector<Option>& options) const;
        void PrintError(const std::string& msg) const;

    private:
        std::string prog_name_;
    };

}  // namespace cli::framework

#endif // CLI_FRAMEWORK_HELP_FORMATTER_H_