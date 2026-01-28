# 如何新增一种 Range 类型的报告 (如与 Year/Week 类似的自定义时间段)

本文档将指导你如何基于现有的 `RangeReportService` 架构，复用已有的逻辑来新增一种时间范围类型的报告（例如：“季度报”、“双周报”等）。

我们以新增 **Year (年报)** 为例，介绍需要修改的各个层级。

---

## 1. 配置层 (Configuration Layer)

首先需要有了该报告类型的配置定义，以便用户可以在 `config` 目录下通过 TOML 文件自定义样式。

### 1.1 创建配置文件
在 `config/reports/` 下创建新目录（如 `year/`），并添加三种格式的默认配置：
*   `YearMdConfig.toml` (Markdown)
*   `YearTypConfig.toml` (Typst)
*   `YearTexConfig.toml` (LaTeX)

### 1.2 更新 C++ 配置模型
在 `src/common/config/global_report_config.hpp`:
1.  在 `FormatReportConfig` 结构体中添加成员变量：
    ```cpp
    struct FormatReportConfig {
      // ...
      RangeReportConfig week_;
      RangeReportConfig year_; // [新增]
    };
    ```

### 1.3 更新配置加载逻辑
在 `src/config/parser_utils.cpp` 的 `parse_report_paths` 函数中：
1.  添加对新 keys (`year`) 的解析 logic。

在 `src/config/config_loader.cpp` 的 `load_detailed_reports` 函数中：
1.  调用 `ReportConfigLoader::loadRange...Config` 将 TOML 文件加载到内存中的 `GlobalReportConfig` 对象。

最后，要在主配置文件 `config/config.toml` 中注册这些新文件的路径。不然会导致解析配置的时候找不到配置文件。config/config.toml这个文件夹和src在同级目录下。

---

## 2. 领域层 (Domain Layer)

### 2.1 定义 RangeType
在 `src/reports/application/usecases/range_report_service.hpp`:
1.  枚举 `RangeType` 中增加 `Year`（以及现有的 `Recent`, `Month`, `Week`, `Range`）。
    ```cpp
    enum class RangeType { Recent, Month, Week, Year, Range };
    ```
    *   **Recent**: 最近 N 天 (如 last 7 days)。
    *   **Range**: 自定义闭区间 (如 2024-01-01 to 2024-01-15)。

### 2.2 定义数据结构
在 `src/core/domain/model/query_data_structs.hpp`:
1.  定义用于导出的数据类型别名，方便后续处理。
    ```cpp
    using FormattedYearlyReports = std::map<int, std::string>;
    ```

### 2.3 更新仓储接口
在 `src/core/domain/repositories/i_report_repository.hpp`:
1.  增加查询所有历史数据的接口（如果支持批量导出）：
    ```cpp
    virtual std::map<std::string, ...> get_all_years_project_stats() = 0;
    virtual std::map<std::string, int> get_all_years_active_days() = 0;
    ```
2.  增加生成报告内容的接口：
    ```cpp
    virtual std::string GetYearlyReport(int year, ReportFormat format) = 0;
    virtual void GetAllYearlyReports(ReportFormat format) = 0;
    ```

---

## 3. 基础设施层 (Infrastructure Layer)

### 3.1 实现数据查询
在 `src/reports/infrastructure/persistence/sqlite_report_data_repository.cpp`:
1.  实现接口 `get_all_years_...`。
2.  编写 SQL 语句按年聚合数据。例如使用 SQLite 的 `strftime('%Y', date)`。

### 3.2 实现适配器
在 `src/core/infrastructure/persistence/sqlite_report_repository_adapter.cpp`:
1.  实现 `GetYearlyReport`：构建 `RangeRequest`，设置 `type = RangeType::Year`，并调用 `range_service_->generate_report(...)`。
2.  实现 `GetAllYearlyReports`：调用 `range_service_->generate_all_yearly_history(...)`。

---

## 4. 应用服务层 (Application Service Layer)

### 4.1 扩展 RangeReportService
在 `src/reports/application/usecases/range_report_service.cpp`:
1.  修改 `get_config_by_format`：根据 `RangeType::Year` 返回对应的 `year_` 配置对象。
2.  实现 `generate_all_yearly_history`：
    *   获取 `Year` 类型配置。
    *   调用 Repo 获取所有数据。
    *   遍历数据，调用 `GenericFormatterFactory` 生成报告字符串。

### 4.2 更新 ReportGenerator
在 `src/application/service/report_generator.cpp`:
1.  添加透传方法 `GenerateYearlyReport` 和 `GenerateAllYearlyReports`，直接调用 Repository 层的对应方法。

---

## 5. 接口与处理层 (Interface & Handler Layer)

### 5.1 更新 ReportHandler
在 `src/application/service/report_handler.cpp`:
1.  实现 `RunYearlyQuery`：调用 Generator 并返回字符串。
2.  实现 `RunExportSingleYearReport`：生成内容后调用 Exporter。
3.  实现 `RunExportAllYearlyReportsQuery`。

---

## 6. 导出层 (Export Layer)

### 6.1 更新 ReportFileManager
在 `src/core/infrastructure/reporting/report_file_manager.cpp`:
1.  定义保存路径规则。例如：`exported_reports/markdown/yearly/2026.md`。

### 6.2 更新 Exporter
在 `src/core/infrastructure/reporting/exporter.cpp`:
1.  实现 `ExportSingleYearReport` 和 `ExportAllYearlyReports`。
2.  调用 `file_manager_` 获取路径并写入文件。

---

## 7. 命令行层 (CLI Layer)

### 7.1 更新 Commands
在 `src/cli/impl/commands/all_commands.cpp`:
1.  **QueryCommand**: 支持 `query day <date>`, `query month <month>`, `query year <year>` 等。
2.  **ExportCommand**: 支持 `export day <date>`, `export year <year>` 和 `export all-year`。
3.  确保参数解析和验证逻辑覆盖了新的类型。

---

## 8. CLI 命名规范 (CLI Naming Convention)

在添加新报告或修改现有命令时，必须遵循以下准则：

> [!IMPORTANT]
> **统一使用名词，不要加 "-ly" 后缀。**

*   **单数报告**: 使用 `day`, `week`, `month`, `year` 而不是 `daily`, `weekly`, `monthly`, `yearly`。
*   **批量报告**: 使用 `all-day`, `all-week`, `all-month`, `all-year` 而不是 `all-daily` 等。
*   **其他**: `recent`, `range` 等也保持名词形式。

这确保了命令行参数的一致性和简洁性。详细信息请参考 [CLI README](../../../apps/time_tracer/src/cli/README.md)。

---

## 总结

复用 `RangeReportService` 的核心在于：
1.  **配置隔离**：通过 `RangeType` 区分使用哪一份配置（Year/Month/Week）。
2.  **通用格式化**：`GenericFormatterFactory` 会根据传入的配置对象自动处理标题和标签，无需为新报表重写格式化器代码。
3.  **数据抽象**：底层只需提供符合标准结构的统计数据 (`RangeReportData`)，上层即可自动渲染。
