// cli/impl/commands/all_commands.hpp
// 合并�? pipeline_commands.hpp, export_command.hpp, query_command.hpp
#ifndef CLI_IMPL_COMMANDS_ALL_COMMANDS_HPP_
#define CLI_IMPL_COMMANDS_ALL_COMMANDS_HPP_

#include "application/interfaces/i_log_converter.hpp"
#include "application/interfaces/i_log_serializer.hpp"
#include "application/interfaces/i_report_handler.hpp"
#include "application/interfaces/i_workflow_handler.hpp"
#include "cli/framework/command.hpp"
#include "common/config/app_config.hpp"
#include "core/domain/ports/i_file_system.hpp"
#include "core/domain/ports/i_user_notifier.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// ============================================================================
// Pipeline Commands - 基于 WorkflowHandler 的命�?
// ============================================================================

class ConvertCommand : public ICommand {
public:
  explicit ConvertCommand(IWorkflowHandler &workflow_handler);
  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  IWorkflowHandler &workflow_handler_;
};

class ImportCommand : public ICommand {
public:
  explicit ImportCommand(IWorkflowHandler &workflow_handler);
  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  IWorkflowHandler &workflow_handler_;
};

class IngestCommand : public ICommand {
public:
  explicit IngestCommand(IWorkflowHandler &workflow_handler);
  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  IWorkflowHandler &workflow_handler_;
};

// ============================================================================
// Validate Commands - 基于 Pipeline 逻辑的校验命�?
// ============================================================================

class ValidateLogicCommand : public ICommand {
public:
  explicit ValidateLogicCommand(
      const AppConfig &config, const std::filesystem::path &output_root,
      std::shared_ptr<core::interfaces::IFileSystem> fs,
      std::shared_ptr<core::interfaces::IUserNotifier> notifier,
      std::shared_ptr<core::interfaces::ILogSerializer> serializer,
      std::shared_ptr<core::interfaces::ILogConverter> converter);

  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  const AppConfig &app_config_;
  std::filesystem::path output_root_;
  std::shared_ptr<core::interfaces::IFileSystem> fs_;
  std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
  std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
  std::shared_ptr<core::interfaces::ILogConverter> converter_;
};

class ValidateStructureCommand : public ICommand {
public:
  explicit ValidateStructureCommand(
      const AppConfig &config, const std::filesystem::path &output_root,
      std::shared_ptr<core::interfaces::IFileSystem> fs,
      std::shared_ptr<core::interfaces::IUserNotifier> notifier,
      std::shared_ptr<core::interfaces::ILogSerializer> serializer,
      std::shared_ptr<core::interfaces::ILogConverter> converter);

  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  const AppConfig &app_config_;
  std::filesystem::path output_root_;
  std::shared_ptr<core::interfaces::IFileSystem> fs_;
  std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
  std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
  std::shared_ptr<core::interfaces::ILogConverter> converter_;
};

// ============================================================================
// Export Command - 报表导出命令
// ============================================================================

class ExportCommand : public ICommand {
public:
  explicit ExportCommand(std::shared_ptr<IReportHandler> report_handler);

  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  std::shared_ptr<IReportHandler> report_handler_;
};

// ============================================================================
// Query Command - 报表查询命令
// ============================================================================

class QueryCommand : public ICommand {
public:
  explicit QueryCommand(std::shared_ptr<IReportHandler> report_handler);

  std::vector<ArgDef> GetDefinitions() const override;
  std::string GetHelp() const override;
  void Execute(const CommandParser &parser) override;

private:
  std::shared_ptr<IReportHandler> report_handler_;
};

#endif // CLI_IMPL_COMMANDS_ALL_COMMANDS_HPP_
