// bootstrap/startup_validator.cpp
#include "bootstrap/startup_validator.hpp"

#include <iostream>
#include <filesystem>
#include <vector>

#include "config/validator/plugins/facade/plugin_validator.hpp"
#include "common/ansi_colors.hpp"

// [新增] 引入磁盘文件系统
#include "io/disk_file_system.hpp"

namespace fs = std::filesystem;

bool StartupValidator::validate_environment(const AppConfig& config) {
    // 1. 定义需要检查的插件列表
    const std::vector<std::string> expected_plugins = {
        "DayMdFormatter", "DayTexFormatter", "DayTypFormatter",
        "MonthMdFormatter", "MonthTexFormatter", "MonthTypFormatter",
        "PeriodMdFormatter", "PeriodTexFormatter", "PeriodTypFormatter"
    };

    // 2. 准备路径
    fs::path plugins_dir = fs::path(config.exe_dir_path) / "plugins";
    fs::path bin_dir = config.exe_dir_path; 

    // 3. 执行验证
    PluginValidator validator;
    
    // [新增] 创建 FS 实例
    io::DiskFileSystem fs;
    
    // 3.1 验证格式化器插件 [修改: 传递 fs]
    bool plugins_ok = validator.validate(fs, plugins_dir, expected_plugins);

    // 3.2 验证核心共享库 [修改: 传递 fs]
    bool core_ok = validator.validate(fs, bin_dir, {"reports_shared"});

    if (!plugins_ok || !core_ok) {
        std::cerr << RED_COLOR << "Fatal: Runtime environment validation failed (Missing DLLs)." << RESET_COLOR << std::endl;
        return false; 
    }

    return true;
}