# Config 模块

配置加载与验证模块，负责从 TOML 文件加载配置并验证其合法性。

## 目录结构

```
config/
├── config_loader.hpp/.cpp      # 顶层入口
├── parser_utils.hpp/.cpp       # TOML 解析工具
├── loaders/                    # 加载器
│   ├── converter_loader.*      # ConverterConfigLoader, TomlConverterConfigLoader
│   └── report_loader.*         # ReportConfigLoader
└── validators/                 # 验证器
    ├── validator_facade.*      # ConfigFacade, ConverterFacade, QueryFacade
    ├── converter_validator.*   # MainRule, MappingRule, DurationRule
    ├── report_validator.*      # IQueryStrategy, BaseStrategy, DailyXxxStrategy, RangeStrategy, StrategyFactory
    └── plugin_validator.*      # PluginValidator
```

## 关键类

| 类名 | 文件 | 职责 |
|------|------|------|
| `ConfigLoader` | config_loader.* | 顶层入口，加载 config.toml 并返回 AppConfig |
| `ConfigParserUtils` | parser_utils.* | 解析 [system], [converter], [reports] 配置节 |
| `TomlLoaderUtils` | parser_utils.* | TOML 读取和类型转换工具 |
| `ConverterConfigLoader` | loaders/converter_loader.* | 从文件加载 ConverterConfig |
| `ReportConfigLoader` | loaders/report_loader.* | 加载 DailyXxxConfig / RangeXxxConfig |
| `ConfigFacade` | validators/validator_facade.* | 验证入口，委托给子验证器 |
| `ConfigValidator::StrategyFactory` | validators/report_validator.* | 根据文件名创建对应的验证策略 |

## 数据流

```
config.toml
    │
    ▼
ConfigLoader::load_configuration()
    │
    ├─► ConfigParserUtils::parse_*()     → AppConfig (基础配置)
    │
    ├─► ConverterConfigLoader::load()    → ConverterConfig
    │
    └─► ReportConfigLoader::load*()      → DailyXxxConfig / RangeXxxConfig
```

## 命名空间

- **全局**: `ConfigLoader`, `ReportConfigLoader`, `ConverterConfigLoader`
- **`ConfigParserUtils`**: 主配置解析
- **`TomlLoaderUtils`**: TOML 工具函数
- **`ConfigValidator`**: 验证策略 (IQueryStrategy, BaseStrategy, StrategyFactory 等)
