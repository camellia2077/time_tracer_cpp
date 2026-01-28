# Reports 模块 (Reports Module)

Reports 模块负责系统中所有的报表生成逻辑，采用了 **领域驱动设计 (DDD)** 和 **整洁架构 (Clean Architecture)** 原则进行重构。

## 架构概览

该模块严格分离了业务逻辑、应用流程、基础设施实现和表现层格式化。

### 目录结构

```
reports/
├── domain/                         # 领域层 (Domain Layer) - 核心业务
│   ├── model/                      # 领域实体与值对象
│   │   ├── daily_report_data.hpp   # 日报数据模型
│   │   ├── range_report_data.hpp   # 范围报表数据模型 (周/月/时段)
│   │   └── project_tree.hpp        # 项目树结构
│   └── repositories/               # 仓储接口定义
│       └── i_report_repository.hpp # 报表数据访问接口
│
├── application/                    # 应用层 (Application Layer) - 业务流程
│   └── usecases/                   # 用例服务
│       ├── daily_report_service.*  # 日报生成服务
│       └── range_report_service.*  # 范围报表生成服务
│
├── infrastructure/                 # 基础设施层 (Infrastructure Layer) - 技术实现
│   └── persistence/                # 持久化实现
│       └── sqlite_report_data_repository.* # SQLite 数据访问实现
│
├── presentation/                   # 表现层 (Presentation Layer) - 格式化与输出
│   ├── daily/                      # 日报格式化器
│   │   └── formatters/             # (Markdown/LaTeX/Typst)
│   └── range/                      # 范围报表格式化器 (周/月/时段)
│       └── formatters/             # (Markdown/LaTeX/Typst)
│
├── shared/                         # 共享组件层 (原 core/)
│   ├── factories/                  # 格式化器工厂
│   │   └── generic_formatter_factory.hpp
│   ├── formatters/                 # 通用格式化器基类
│   │   ├── base/
│   │   ├── markdown/
│   │   ├── latex/
│   │   └── typst/
│   ├── config/                     # 样式配置
│   │   └── export_style_config.*
│   ├── utils/                      # 通用工具
│   │   ├── report_string_utils.*
│   │   └── report_time_format.*
│   └── traits/                     # 类型特征
│
└── data/                           # 数据辅助层
    ├── cache/                      # 缓存 (如 ProjectNameCache)
    ├── queriers/                   # 具体的查询对象 (Querier Objects)
    └── utils/                      # 数据构建工具 (ProjectTreeBuilder)
```

## 核心原则

1.  **依赖倒置 (Dependency Inversion)**:
    *   `Application` 依赖 `Domain`。
    *   `Infrastructure` 依赖 `Domain` (实现接口)。
    *   `Presentation` 依赖 `Domain` (使用模型)。
    *   `Domain` 层不依赖任何外层模块。
    *   共享类型 (`ReportFormat`, `QueryDataStructs`) 已移至 `core/domain`。

2.  **关注点分离**:
    *   **Domain**: 定义 "什么是报表数据"。
    *   **Infrastructure**: 解决 "数据从哪里来" (SQLite)。
    *   **Application**: 协调 "如何获取并处理数据"。
    *   **Presentation**: 负责 "数据长什么样" (Markdown/Typst)。
    *   **Shared**: 提供通用的格式化器和工具。

## 关键类说明

*   **Entities (`domain/model`)**: 纯净的数据结构，无逻辑。
*   **Services (`application/usecases`)**: 包含核心业务逻辑，由于 CLI 直接调用，目前可能包含一些协调逻辑。
*   **Formatters (`presentation/**`)**: 负责将 Domain Model 转换为最终字符串。继承自通用基类。
*   **Repository (`infrastructure/persistence`)**: 封装 SQL 查询，对外提供 Domain Model。
*   **Shared (`shared/**`)**: 原 `reports/core`，提供通用格式化器基类和工具。

## 依赖关系

- ✅ 依赖: `core/domain/types` (ReportFormat), `core/domain/model` (QueryDataStructs)
- ❌ 不依赖: application 层、config 层
