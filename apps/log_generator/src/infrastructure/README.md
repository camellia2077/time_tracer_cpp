# Infrastructure Layer (基础设施层)

## Overview
基础设施层包含所有技术相关的具体实现，是 Domain 层接口的实现者。

## Directory Structure
```
infrastructure/
├── concurrency/               # 并发相关
│   └── thread_pool_executor.* # ITaskExecutor 实现
├── persistence/               # 持久化相关
│   ├── file_manager.*         # IFileReader/IFileWriter 实现
│   └── toml_config_parser.*   # TOML 配置解析器
└── factories/                 # 工厂实现
    └── log_generator_factory.hpp  # ILogGeneratorFactory 实现
```

## Design Principles

### 1. 接口实现
- 所有类都实现 Domain 层或 Application 层定义的接口
- 遵循依赖倒置原则（DIP）

### 2. 技术细节封装
- 文件系统操作、TOML 解析、线程池等技术细节封装在此层
- 上层模块无需关心这些技术实现

### 3. 可替换性
- 通过接口隔离，可以轻松替换实现
- 例如：可以用不同的线程池实现替换 `ThreadPoolExecutor`

## Interface Mapping

| Interface | Implementation |
|-----------|----------------|
| `Domain::Ports::IFileReader` | `Infrastructure::FileManager` |
| `Domain::Ports::IFileWriter` | `Infrastructure::FileManager` |
| `Domain::Ports::ITaskExecutor` | `Infrastructure::ThreadPoolExecutor` |
| `Application::Factories::ILogGeneratorFactory` | `Infrastructure::LogGeneratorFactory` |

## Namespace
所有类型位于 `Infrastructure` 命名空间下。
