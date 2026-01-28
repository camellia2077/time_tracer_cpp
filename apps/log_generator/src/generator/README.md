# Generator Module (生成器模块)

## Overview
生成器模块是 log_generator 的核心业务逻辑模块，负责生成模拟的时间日志内容。

## Directory Structure
```
generator/
├── log_generator.hpp/.cpp     # ILogGenerator 接口实现
├── components/                # 子组件
│   ├── day_generator.*        # 单日日志生成器
│   ├── event_generator.*      # 事件生成器
│   └── remark_generator.*     # 备注生成器
└── strategies/                # 策略
    └── sleep_scheduler.*      # 睡眠/通宵调度策略
```

## Design Patterns

### 1. 组合模式
- `LogGenerator` 组合 `DayGenerator` 和 `SleepScheduler`
- `DayGenerator` 组合 `EventGenerator` 和 `RemarkGenerator`

### 2. 策略模式
- `SleepScheduler` 封装通宵日的调度算法
- 可以轻松替换不同的调度策略

### 3. 接口实现
- `LogGenerator` 实现 `Domain::Ports::ILogGenerator`
- 通过接口与 Application 层解耦

## Thread Safety
- 每个 `LogGenerator` 实例包含独立的 `std::mt19937` 随机数引擎
- **不可跨线程共享实例**
- 应通过 Factory 在每个线程/任务中创建新实例

## Namespace
所有类型位于 `Generator` 命名空间下。
