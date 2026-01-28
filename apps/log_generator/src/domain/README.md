# Domain Layer (领域层)

## Overview
领域层是 log_generator 的核心层，包含纯粹的业务模型和接口定义，**不依赖任何外部库或技术细节**。

## Directory Structure
```
domain/
├── model/                  # 领域模型（实体、值对象）
│   ├── config.hpp          # 运行时配置
│   ├── remark_config.hpp   # 备注配置
│   └── generation_context.hpp  # 生成上下文（聚合根）
└── ports/                  # 端口接口（依赖倒置的契约）
    ├── i_log_generator.hpp # 日志生成器接口
    ├── i_file_system.hpp   # 文件系统接口
    └── i_task_executor.hpp # 任务执行器接口
```

## Design Principles

### 1. 零外部依赖
- 本层仅使用 C++ 标准库
- 不包含任何第三方库头文件

### 2. 依赖倒置 (DIP)
- `ports/` 定义了 Application 层需要的技术能力
- 具体实现在 `infrastructure/` 层提供
- Application 层依赖接口，不依赖实现

### 3. 聚合根
- `GenerationContext` 是本领域的聚合根
- 它聚合了生成日志所需的全部配置信息

## Usage
```cpp
// Application 层依赖 domain 接口
#include "domain/ports/i_log_generator.hpp"
#include "domain/model/generation_context.hpp"

// Infrastructure 层实现 domain 接口
class LogGenerator : public Domain::Ports::ILogGenerator { ... };
```

## Namespace
所有类型位于 `Domain::Model` 或 `Domain::Ports` 命名空间下。
