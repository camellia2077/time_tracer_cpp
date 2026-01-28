# CLI Layer (命令行界面层)

## Overview
CLI 层是 log_generator 的表现层，负责用户交互、参数解析和帮助信息展示。

## Directory Structure
```
cli/
├── log_generator_cli.hpp/.cpp  # CLI 主入口
└── framework/                   # CLI 框架组件
    ├── arg_parser.*             # 参数解析器
    ├── help_formatter.*         # 帮助信息格式化
    ├── parsed_args.hpp          # 解析结果容器
    └── ansi_colors.hpp          # 终端颜色定义
```

## Design Principles

### 1. 职责分离
- `LogGeneratorCli`: 高层入口，协调解析、构建、校验流程
- `ArgParser`: 通用参数解析框架
- `HelpFormatter`: 帮助信息生成

### 2. 输入输出
- **输入**: 命令行参数 (`argc`, `argv`)
- **输出**: `Domain::Model::Config` 对象 或 `std::nullopt`

### 3. 依赖方向
- CLI 层依赖 Domain 层的模型（`Config`）
- CLI 层不依赖 Application 或 Infrastructure 层

## Supported Options

| Short | Long | Description |
|-------|------|-------------|
| `-h` | `--help` | 显示帮助信息 |
| `-v` | `--version` | 显示版本信息 |
| `-y` | `--year` | 生成单年数据 |
| `-s` | `--start` | 起始年份 |
| `-e` | `--end` | 结束年份 |
| `-i` | `--items` | 每天事件数（默认 10） |
| `-n` | `--nosleep` | 启用通宵日 |

## Namespace
所有类型位于 `Cli` 或 `Cli::Framework` 命名空间下。
