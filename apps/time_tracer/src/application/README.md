# Application 模块 (应用层)

应用层 (Application Layer) 是系统的**业务编排中心**。它负责接收外部请求（命令），协调领域对象 (Core/Domain) 和基础设施 (Core/Infrastructure) 来完成具体的业务用例。

## 职责边界

*   **编排与调度**: 不包含具体的业务规则（属于 Domain）或底层实现（属于 Infrastructure），而是像交警一样指挥各模块协同工作。
*   **命令处理 (CQRS)**: 通过 Command/Handler 模式处理用户请求 (`IngestCommand`, `ExportCommand`)。
*   **流水线执行 (Pipeline)**: 管理数据处理流水线 (`PipelineRunner`, `Steps`)，定义数据流转的步骤。
*   **对外接口**: 定义应用层需要的技术端口 (`Ports`)，如 `IFileSystem`, `IUserNotifier`。

## 目录结构

```
application/
├── commands/               # 命令对象 (DTO)
│   ├── ingest_command.hpp  # 数据入库命令
│   └── export_command.hpp  # 数据导出命令
│
├── handlers/               # 命令处理器 (业务入口)
│   ├── ingest_handler.*    # 处理 IngestCommand -> 驱动 Pipeline
│   └── export_handler.*    # 处理 ExportCommand -> 驱动 Repositories
│
├── pipeline/               # 核心流水线框架
│   ├── context/            # Pipeline 上下文
│   ├── interfaces/         # Step 接口
│   └── ...
│
├── steps/                  # 具体 Pipeline 步骤
│   ├── file_collector.*    # 文件收集
│   ├── converter_step.*    # 转换步骤
│   └── ...
│
├── service/                # 遗留服务 (逐步迁移至 Handlers)
│   └── workflow_handler.*
│
├── ports/                  # 端口接口 (供 Infrastructure 实现)
│   ├── i_file_system.hpp
│   └── i_user_notifier.hpp
│
└── utils/                  # 应用层工具
```

## 核心模式：Command Handler

应用层推荐使用 **Command/Handler** 模式作为业务入口：

```cpp
// 1. 定义意图 (Command)
application::commands::IngestCommand cmd;
cmd.source_path = "...";

// 2. 处理意图 (Handler)
application::handlers::IngestHandler handler(deps...);
auto result = handler.handle(cmd);
```
