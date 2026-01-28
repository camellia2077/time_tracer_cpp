# Application 模块 (应用层)

应用层 (Application Layer) 是系统的**业务编排中心**。它负责接收外部请求（命令），协调领域对象 (Core/Domain) 和基础设施 (Core/Infrastructure) 来完成具体的业务用例。

## 职责边界

*   **编排与调度**: 不包含具体的业务规则（属于 Domain）或底层实现（属于 Infrastructure），而是像交警一样指挥各模块协同工作。
*   **命令处理 (CQRS)**: 通过 Command/Handler 模式处理用户请求 (`IngestCommand`, `ExportCommand`)。
*   **流水线执行 (Pipeline)**: 管理数据处理流水线 (`PipelineRunner`, `Steps`)，定义数据流转的步骤。
*   **依赖倒置**: 依赖 `core/domain/ports` 中定义的接口 (如 `IFileSystem`, `IUserNotifier`)，不直接依赖具体实现。

## 目录结构

```
application/
├── pipeline/               # 核心流水线框架
│   ├── factory.*           # Pipeline 工厂
│   ├── runner.*            # Pipeline 运行器
│   ├── context/            # Pipeline 上下文
│   └── interfaces/         # Step 接口
│
├── commands/               # 命令对象 (DTO)
│   ├── ingest_command.hpp  # 数据入库命令
│   └── export_command.hpp  # 数据导出命令
│
├── handlers/               # 命令处理器 (业务入口)
│   ├── ingest_handler.*    # 处理 IngestCommand -> 驱动 Pipeline
│   └── export_handler.*    # 处理 ExportCommand -> 驱动 Repositories
│
├── service/                # 业务服务
│   ├── report_handler.*    # 报表处理服务
│   ├── report_generator.*  # 报表生成器
│   └── workflow_handler.*  # 工作流处理器
│
├── steps/                  # 具体 Pipeline 步骤
│   ├── file_collector.*    # 文件收集
│   ├── converter_step.*    # 转换步骤
│   └── ...
│
├── interfaces/             # 应用层接口
│   ├── i_report_handler.hpp
│   └── i_workflow_handler.hpp
│
└── utils/                  # 应用层工具
    ├── converter_config_factory.*
    └── processed_data_writer.*
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

## 依赖关系

- ✅ 依赖: `core/domain/ports`, `core/domain/interfaces`, `core/domain/model`
- ❌ 不依赖: 具体的数据库实现、文件系统实现
