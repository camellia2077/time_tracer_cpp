// cli/impl/app/cli_application.hpp
#ifndef CLI_IMPL_APP_CLI_APPLICATION_HPP_
#define CLI_IMPL_APP_CLI_APPLICATION_HPP_

#include <vector>
#include <string>
#include <memory>
#include <filesystem>

#include "cli/framework/core/command_parser.hpp"
#include "common/config/app_config.hpp"

// 前向声明
struct AppContext;
class DBManager;

class CliApplication {
public:
    explicit CliApplication(const std::vector<std::string>& args);
    ~CliApplication();

    void execute();

private:
    void initialize_output_paths();

    CommandParser parser_;
    AppConfig app_config_;
    
    std::shared_ptr<AppContext> app_context_;
    std::unique_ptr<DBManager> db_manager_;
    
    std::filesystem::path output_root_path_;
    std::filesystem::path exported_files_path_;
};

#endif // CLI_IMPL_APP_CLI_APPLICATION_HPP_