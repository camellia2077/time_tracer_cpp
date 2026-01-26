# 架构与模块 (Architecture & Modules)
**核心设计理念**：
TimeTracer 采用**模块化单体 (Modular Monolith)** 架构，以 **Core Orchestration** 为核心，实现了业务逻辑与 IO、UI 的彻底解耦，旨在构建一个可插拔、易于跨平台移植的量化生活引擎。
## 0. 系统分层与职责边界 (System Layers & Responsibilities)

为了确保系统的健壮性与可维护性，系统采用了严格的分层架构，各层职责互不越界。

### 0.1 启动引导层 (Bootstrap Layer)
* **核心组件**: `bootstrap/StartupValidator`
* **职责**: 系统的“门卫”。
    * **环境完整性检查**: 确保所有必要的依赖（如 `reports_shared.dll` 等插件）已就绪。
    * **配置文件加载**: 读取物理配置文件，但不进行深入的业务规则校验。
    * **生命周期控制**: 如果环境不满足运行条件，直接终止程序启动，防止核心业务在不稳定环境中运行。

### 0.2 配置校验层 (Validation Layer)
* **核心组件**: `config_validator/facade/ConfigFacade`
* **职责**: 纯粹的规则验证引擎。
    * **业务规则检查**: 验证加载的 JSON 配置是否符合业务规范（如必填字段、数值范围、颜色格式）。
    * **无状态性**: 只接收数据对象进行判断，不负责 I/O 操作。

### 0.3 核心业务编排层 (Core Orchestration Layer)
* **核心组件**: `core` 
* **组件示例**: `WorkflowHandler`, `PipelineManager`
* **职责**: 
    * **信任原则**: 该层**假设**环境是健康的、配置是合法的（由上层保证）。
    * **流程编排**: 专注于调度预处理、数据库转换、报表生成等核心业务逻辑。
    * **数据校验**: 仅关注**用户数据**（如日志内容）的合法性，不关注**系统配置**的合法性。

### 0.4 跨平台核心引擎设计 (Cross-Platform Core Engine)
* **设计理念**: 类似于现代工业软件（如 Photoshop, VS Code）的 "Stub Executable" 模式。
* **Core 的角色**: 
    * **无头模式 (Headless)**: Core 编译为动态库 (DLL/SO)，不包含任何 UI 代码，也不直接依赖具体的渲染引擎（如 LaTeX/Typst 具体实现）。
    * **调度中心**: Core 仅负责下达指令（Ingest, Export），具体的脏活（解析、格式化）由底层模块完成。
* **API 边界**: 
    * Core 仅向外部暴露 **基础类型 (String/JSON)** 或 **状态码**。
    * 严禁将内部 C++ 复杂对象（如 `DailyReportData`）暴露给 UI 层，确保未来适配 Flutter/Electron/Native UI 时实现“一次编写，到处运行”。

## 1. 系统概览与数据流 (System Overview & Data Flow)

本系统的核心工作流由两个单向流动的阶段组成：**数据入库 (Data Ingestion)** 与 **报告生成 (Report Generation)**。

### 1.1 第一阶段：数据入库 (Ingestion Phase)

此阶段负责将用户用txt记录的的非结构化内容，转换为结构化数据(Structs)，并持久化存储到关系型数据库中。

流程：`txt` -> `Structs` -> `SQLite` (可选生成 JSON)

* **原始解析 (Raw Processing)**:
* **输入**: 用户编写的原始时间日志文本文件 (`.txt`)。
* **处理**: 系统按行读取文本，应用解析规则识别时间戳、活动名称及备注。在此过程中，系统会自动处理业务逻辑（如跨天睡眠的分割、活动时长的计算），并将每日数据标准化为内存对象。
* **中间产物**: (可选) 生成包含完整元数据和统计信息的标准化 **JSON**，仅作为数据清洗的归档或人工检查点，不再是入库的必经环节。



### 1.2 第二阶段：报告生成 (Reporting Phase)

此阶段负责从数据库中提取数据，重建业务模型，并渲染为用户所需的最终格式。

流程：`SQLite` -> `Flat Records` -> `ProjectTree (Memory)` -> `Formatted String(md tex typ)`

* **数据查询与重建 (Query & Reconstruction)**:
* **输入**: 用户的查询请求（如“生成某月月报”）。
* **处理**: 系统根据时间范围查询 SQLite 数据库，获取扁平化的记录列表。随后，系统在内存中将这些零散的记录**动态聚合**，重建为一棵带有统计数据的 **项目树 (Project Tree)**。这棵树在逻辑上等同于一个多层嵌套的 JSON 对象。


* **渲染输出 (Rendering)**:
* **处理**: 渲染引擎遍历内存中的项目树。
* **策略应用**: 根据用户指定的格式（Markdown, Typst, LaTeX），系统加载对应的**格式化策略**。在遍历树节点的同时，策略模块将节点数据转换为目标格式的字符串。
* **输出**: 生成最终的可视化报告文本。



