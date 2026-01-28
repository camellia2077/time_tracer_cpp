# Application Layer (应用层)

## Overview
应用层负责编排业务用例，协调 Domain 层和 Infrastructure 层完成具体的业务流程。

## Directory Structure
```
application/
├── application.hpp/.cpp     # 顶层应用编排器
├── handlers/                # 用例处理器
│   ├── config_handler.*     # 配置加载用例
│   └── workflow_handler.*   # 工作流执行用例
└── factories/               # 抽象工厂接口
    └── i_log_generator_factory.hpp
```

## Design Principles

### 1. 用例驱动
- 每个 Handler 对应一个具体的业务用例
- `ConfigHandler`: 加载并合并配置
- `WorkflowHandler`: 执行日志生成工作流

### 2. 依赖倒置
- Application 层仅依赖 Domain 层的接口（`domain/ports/`）
- 具体实现通过构造函数注入（依赖注入模式）

### 3. 编排而非实现
- 本层不包含具体的业务逻辑或技术细节
- 仅负责"调用谁、何时调用、以什么顺序调用"

## Dependency Flow
```
main.cpp
    ↓
Application (组装依赖)
    ↓
ConfigHandler → 加载配置 → GenerationContext
    ↓
WorkflowHandler → 执行生成 → ILogGenerator (via Factory)
```

## Usage
```cpp
#include "application/application.hpp"

int main(int argc, char* argv[]) {
    Application::Application app;
    return app.run(argc, argv);
}
```

## Namespace
所有类型位于 `Application`、`Application::Handlers` 或 `Application::Factories` 命名空间下。
