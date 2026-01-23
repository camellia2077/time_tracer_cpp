// cli/framework/help_formatter.hpp
#ifndef CLI_FRAMEWORK_HELP_FORMATTER_HPP_
#define CLI_FRAMEWORK_HELP_FORMATTER_HPP_

#include <string>
#include <vector>

namespace Cli::Framework {

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

        void print_help(const std::vector<Option>& options) const;
        void print_error(const std::string& msg) const;

    private:
        std::string prog_name_;
    };

}

#endif // CLI_FRAMEWORK_HELP_FORMATTER_HPP_