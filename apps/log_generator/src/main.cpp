// main.cpp
#include "core/application.hpp"
int main(int argc, char* argv[]) {
    // [Core Layer] 启动核心逻辑
    Core::Application app;

    // 直接传递原始命令行参数
    // Application 内部会调用 ConfigHandler -> LogGeneratorCli 进行解析、帮助打印或错误处理
    return app.run(argc, argv);
}