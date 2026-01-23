// core/application.hpp
#ifndef CORE_APPLICATION_HPP_
#define CORE_APPLICATION_HPP_

// [修改] 移除了 Config 的依赖，因为现在接收的是原始参数
// #include "common/config_types.hpp" 
#include <filesystem>

namespace Core {
    class Application {
    public:
        /**
         * @brief 启动核心应用流程。
         * * [修改] 签名已更新以匹配 .cpp 实现。
         * Application 内部负责协调 ConfigHandler 进行参数解析。
         * * @param argc 命令行参数数量
         * @param argv 命令行参数数组
         */
        int run(int argc, char* argv[]);
    };
}

#endif // CORE_APPLICATION_HPP_