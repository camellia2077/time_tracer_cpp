// main.cpp
#include "core/application.hpp"

int main(int argc, char* argv[]) {
    // [Core Layer] 初始化应用实例
    // Application 构造函数或 run 方法内部会负责：
    // 1. 设置控制台环境 (Utils::setup_console)
    // 2. 组装依赖 (Io, ThreadPool, GeneratorFactory)
    // 3. 调用 CLI 解析
    // 4. 执行 Workflow
    Core::Application app;

    // 运行主循环，返回状态码 (0: Success, 1: Error)
    return app.run(argc, argv);
}