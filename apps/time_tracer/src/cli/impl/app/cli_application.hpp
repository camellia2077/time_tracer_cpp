// cli/impl/app/cli_application.hpp
#ifndef CLI_IMPL_APP_CLI_APPLICATION_HPP_
#define CLI_IMPL_APP_CLI_APPLICATION_HPP_

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "cli/framework/core/command_parser.hpp"
#include "common/config/app_config.hpp" 
#include "cli/impl/app/app_context.hpp"

class DBManager;

namespace fs = std::filesystem;

class CliApplication {
public:
    explicit CliApplication(const std::vector<std::string>& args);
    ~CliApplication();

    void execute();

private:
    CommandParser parser_;
    AppConfig app_config_;

    // --- 服务容器 ---
    std::shared_ptr<AppContext> app_context_;

    // --- 基础设施 ---
    // FileController 已被移除，文件操作直接通过 IFileSystem 接口在各模块内部进行
    std::unique_ptr<DBManager> db_manager_;

    fs::path output_root_path_;
    fs::path exported_files_path_;

    void initialize_output_paths();
};

#endif // CLI_IMPL_APP_CLI_APPLICATION_HPP_