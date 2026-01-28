# Core 模块 (核心层)

核心层 (Core Layer) 是系统的**基石**，包含纯净的业务逻辑定义 (Domain) 和具体的底层技术实现 (Infrastructure)。

## 职责边界

*   **Domain (领域层)**: 最内层，不依赖任何外部库（如 SQLite, JSON）。定义系统的核心实体（如 `DailyLog`）、业务规则接口和技术端口接口。它是系统的"真理"。
*   **Infrastructure (基础设施层)**: 最外层，依赖外部库。负责实现 Domain 定义的接口，处理数据库读写、文件 IO、第三方服务调用等"脏活"。

## 目录结构

```
core/
├── domain/                         # 领域层 (无外部依赖)
│   ├── model/                      # 领域实体
│   │   ├── daily_log.hpp           # 每日日志模型
│   │   ├── time_data_models.hpp    # 时间数据模型
│   │   └── query_data_structs.hpp  # 查询结果通用结构 (从 reports 移入)
│   │
│   ├── types/                      # 共享类型定义
│   │   └── report_format.hpp       # 报表格式枚举 (从 reports 移入)
│   │
│   ├── ports/                      # 技术端口接口 (从 application 移入)
│   │   ├── i_file_system.hpp       # 文件系统接口
│   │   └── i_user_notifier.hpp     # 用户通知接口
│   │
│   ├── interfaces/                 # 业务抽象接口
│   │   ├── i_log_repository.hpp    # 持久化接口
│   │   ├── i_log_converter.hpp     # 转换器接口
│   │   └── i_log_serializer.hpp    # 序列化接口
│   │
│   └── repositories/               # 仓库接口
│       └── i_report_repository.hpp # 报表数据访问接口
│
└── infrastructure/                 # 基础设施层 (具体实现)
    ├── persistence/                # 持久化实现 (实现 i_log_repository)
    │   ├── db_manager.*            # SQLite 连接管理
    │   └── sqlite_report_repository_adapter.*
    │
    ├── reporting/                  # 报表生成设施
    │   ├── exporter.*              # 导出器
    │   ├── export_utils.*          # 导出工具
    │   └── report_file_manager.*   # 报表文件管理
    │
    └── services/                   # 服务实现
        └── import_service.*        # 导入服务
```

## 接口分层原则

| 层级 | 目录 | 接口类型 | 作用 |
|------|------|----------|------|
| Domain | `domain/ports/` | 技术端口 | 定义文件系统、用户通知等技术能力接口 |
| Domain | `domain/interfaces/` | 业务接口 | 定义"**要做什么**" (What)，不关心"**怎么做**" (How) |
| Domain | `domain/types/` | 共享类型 | 跨模块共享的枚举和类型定义 |
| Infrastructure | `infrastructure/` | 实现类 | 定义"**怎么做**" (How)，例如"用 SQLite 存数据" |

## 依赖倒置原则 (DIP)

`Application` 依赖 `Domain` 接口，而 `Infrastructure` 实现 `Domain` 接口。运行时通过依赖注入将 `Infrastructure` 的实例注入给 `Application`，从而保证 `Application` 不直接依赖具体的数据库实现。


## 依赖关系

- ✅ domain 层无外部依赖
- ✅ infrastructure 依赖 domain 接口
- ❌ domain 不依赖 infrastructure、application、reports
