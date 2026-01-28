// application/application.hpp
// 应用层：顶层应用编排器
#pragma once
#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

namespace application {

/**
 * @class App
 * @brief 应用程序主入口，协调 CLI 解析、配置加载和工作流执行
 * 
 * 职责：
 * 1. 组装依赖（IoC 容器角色）
 * 2. 协调各层完成业务用例
 * 3. 统一异常处理和程序退出
 * 
 * 注意：类名使用 App 而非 Application，避免与命名空间同名产生歧义
 */
class App {
public:
    App() = default;
    ~App() = default;

    /**
     * @brief 应用程序主入口
     * @return 退出代码 (0 为成功)
     */
    int Run(int argc, char* argv[]);
};

}  // namespace application

#endif  // APPLICATION_APPLICATION_H_
