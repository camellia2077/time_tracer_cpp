# Core 模块 (核心层)

核心层 (Core Layer) 是系统的**基石**，包含纯净的业务逻辑定义 (Domain) 和具体的底层技术实现 (Infrastructure)。

## 职责边界

*   **Domain (领域层)**: 最内层，不依赖任何外部库（如 SQLite, JSON）。定义系统的核心实体（如 `DailyLog`）和业务规则接口（如 `ILogRepository`）。它是系统的“真理”。
*   **Infrastructure (基础设施层)**: 最外层，依赖外部库。负责实现 Domain 定义的接口，处理数据库读写、文件 IO、第三方服务调用等“脏活”。

## 目录结构

```
core/
├── domain/                         # 领域层 (无外部依赖)
│   ├── model/                      # 领域实体
│   │   ├── daily_log.hpp           # 每日日志模型
│   │   └── time_data_models.hpp    # 时间数据模型
│   ├── interfaces/                 # 业务抽象接口
│   │   ├── i_log_repository.hpp    # 持久化接口
│   │   ├── i_log_converter.hpp     # 转换器接口
│   │   └── i_log_serializer.hpp    # 序列化接口
│   └── repositories/               # 仓库接口
│       └── i_report_repository.hpp
│
└── infrastructure/                 # 基础设施层 (具体实现)
    ├── persistence/                # 持久化实现 (实现 i_log_repository)
    │   ├── db_manager.hpp          # SQLite 连接管理
    │   └── ...
    ├── reporting/                  # 报表生成设施
    └── services/                   # 服务实现
```

## 接口分层原则

| 层级 | 目录 | 接口类型 | 作用 |
|------|------|----------|------|
| Domain | `domain/interfaces/` | 业务接口 | 定义“**要做什么**” (What)，不关心“**怎么做**” (How)。 |
| Infrastructure | `infrastructure/` | 实现类 | 定义“**怎么做**” (How)，例如“用 SQLite 存数据”。 |

**依赖倒置原则 (DIP)**:
`Application` 依赖 `Domain` 接口，而 `Infrastructure` 实现 `Domain` 接口。运行时通过依赖注入将 `Infrastructure` 的实例注入给 `Application`，从而保证 `Application` 不直接依赖具体的数据库实现。
