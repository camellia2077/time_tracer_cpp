[English Version](README.en.md)
# time tracer
> **TimeTracer 采用模块化单体 (Modular Monolith) 架构，以 Core Orchestration 为核心，实现了业务逻辑与 IO、UI 的彻底解耦，旨在构建一个可插拔、易于跨平台移植的量化生活引擎。**

**time tracer** 

是一套功能强大的个人时间管理与数据可视化工具集，旨在帮助您精确追踪、分析并优化您的时间利用。

本套件包含三个核心组件：

  * **`time_taster` (C++)**: 核心命令行程序，负责解析原始日志、将数据存入数据库，并提供丰富的查询与报告导出功能。
  * **`graph_generator` (Python)**: 数据可视化工具，能够读取 `Time_Master` 生成的数据库，并创建多种图表，如每日时间线、活动热力图等。
  * **`log_generator` (C++)**: 一个便捷的测试数据生成器，用于快速创建符合格式的日志文件。

-----

## 🚀 快速开始

### 1\. 依赖项

在开始之前，请确保您的系统已安装以下依赖：

  * **C++ 部分 (`time_tracer`, `log_generator`)**:
      * **MSYS2 UCRT64** 环境 (推荐用于 Windows)
      * **CMake** \>= 3.10
      * **GCC** (支持 C++23)
      * **SQLite3** 库
      * **tomlplusplus** 库
  * **Python 部分 (`graph_generator`)**:
      * **Python** \>= 3.8
      * **Matplotlib** 库

### 2\. 编译与安装

我们为所有C++组件提供了详细的编译指南，包括如何配置MSYS2 UCRT64环境和安装必要的库。

➡️ **详细步骤请参考：暂时还没有

### 3\. 基本使用示例

以下是 `time_taster` 命令行工具的一些常用命令，让您快速感受其功能。

**示例 1：完整处理数据**
(检验源文件 -\> 转换 -\> 检验输出 -\> 存入数据库)

```bash
# 假设您的原始日志放在 "raw_logs" 文件夹下
time_tracker_cli -a "path/to/your/raw_logs"
```

**示例 2：查询指定日期的报告**

```bash
# 查询 2025年7月21日 的日报，并以 Markdown 格式输出
time_tracker_cli -q d 20250721 -f md
```

**示例 3：查询上个月的报告**

```bash
# 查询 2025年6月 的月报，并以 LaTeX 格式输出
time_tracker_cli -q m 202506 -f tex
```
## AI 辅助

TimeTracer 定义了一套极简的 **StreamLog 协议**。您无需安装本软件，只需将您的文本日志连同我们提供的 [系统提示词](prompts/streamlog_protocol.md) 发送给任意 LLM (如 ChatGPT, Claude, DeepSeek)，即可获得即时的时间分析。

**使用场景对比：**

| 维度 | TimeTracer Core (C++ Native) | AI 辅助解析 (LLM) |
| :--- | :--- | :--- |
| **核心优势** | **绝对精准、极速、隐私安全** | **自然语言理解、无需安装、容错性强** |
| **计算原理** | 确定性算法 (Deterministic) | 概率性生成 (Probabilistic) |
| **准确率** | **100%** (严格遵循规则) | **~95%** (存在幻觉风险，大数求和可能出错) |
| **耗时** | 毫秒级 (< 10ms) | 秒级/分钟级 (网络延迟+Token生成) |
| **数据隐私** | 本地离线 (Local Offline) | 需上传至云端模型 |
| **适用场景** | 长期归档、精准量化、年报统计 | 临时查看、快速回顾、语义咨询 |

> **最佳实践**：日常使用 AI 进行快速语义回顾；定期使用 TimeTracer Core 进行精准数据清洗与归档。
B. 修改 SystemDesign.md (面向开发者/架构师)
在架构文档中，你需要从技术原理上解释为什么 AI 永远无法取代数据库层。

建议位置：在 1. 设计哲学 或 未来规划 部分。

建议添加内容：

Markdown
### 1.X 确定性引擎与概率性接口的共存 (Determinism vs. Probabilism)

系统引入 AI 提示词协议 (`prompts/streamlog_protocol.md`) 作为一种从属的、可选的交互接口，但绝不替代核心引擎。

1.  **算术精度的不可逾越鸿沟**:
    * LLM 本质是基于 Token 预测的文本生成模型，而非计算引擎。在处理“一整年的分钟数累加”这类任务时，LLM 经常出现算术幻觉。
    * **Core 引擎** 基于 C++ `std::chrono` 和 SQLite 聚合函数，在数学逻辑上保证了 **0 误差**。

2.  **I/O 吞吐与延迟**:
    * **数据库**: 处理 10 年数据（约 10万行）仅需毫秒级 I/O，是实时的。
    * **AI**: 处理同样的数据量需要消耗数百万 Token，耗时数分钟，且伴随高昂的推理成本。

3.  **数据主权**:
    * Core 引擎保证了数据完全驻留本地 (`Local-First`)。AI 解析仅作为一种“只读视图”或“临时辅助工具
-----

## 📚 详细文档

想要深入了解本项目吗？

关于本项目的**详细架构图**、**完整的命令参考**、**API文档**和**各模块使用示例**，请查阅docs/
```
docs/
├── compilation_guide.md          # 详细的编译步骤 
│
├── time_master/                  # Time_Master主程序的专属文档
│   ├── architecture.md           # 包含目录结构和架构图 
│   └── usage.md                  # 包含完整的命令行参数表格和说明
│
├── graph_generator/              # 图表生成器的专属文档
│   └── usage.md                  # 包含其结构、命令和使用示例 
│
└── log_generator/                # 日志生成器的专属文档
    └── usage.md                  # 包含其结构、用法和示例 
```

-----

## 致谢与许可证

本项目的实现离不开以下这些出色的开源库。我向这些项目的开发者们表示感谢！

* **[SQLite C Library](https://www.sqlite.org/index.html)**: 用于数据存储 (Public Domain)。
* **[tomlplusplus](https://github.com/marzer/tomlplusplus)**: 用于解析 TOML 配置文件 (MIT License)。
* **[yyjson](https://github.com/ibireme/yyjson)**: 用于高性能 JSON 解析与生成 (MIT License)。
* **[Matplotlib](https://matplotlib.org/)**: 用于数据可视化 (BSD-style License)。

### 字体声明 (Fonts)
本程序生成的排版文件（如 Typst/LaTeX）默认引用了以下字体：
* **[MiSans](https://hyperos.xiaomi.com/font-download)**: 由小米公司设计并拥有的知识产权，根据其《MiSans 字体知识产权许可协议》免费商用。
* **[Noto Serif SC](https://fonts.google.com/noto/specimen/Noto+Serif+SC)**: 由 Google 与 Adobe 合作开发的开源衬线字体，采用 [SIL Open Font License 1.1](https://scripts.sil.org/OFL) 协议。

