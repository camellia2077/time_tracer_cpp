// cli/impl/commands/pipeline/validate_structure_command.cpp
#include "cli/impl/commands/pipeline/validate_structure_command.hpp"
#include "cli/framework/core/command_parser.hpp"
#include "cli/framework/core/command_validator.hpp"
#include "common/app_options.hpp"
// [修改] 引入新的 Factory 和 Context
#include "core/application/pipeline_factory.hpp"
#include "core/application/pipeline/context/pipeline_context.hpp"

#include "cli/framework/core/command_registry.hpp"
#include "cli/impl/app/app_context.hpp"

static CommandRegistrar<AppContext> registrar("validate-structure", [](AppContext& ctx) {
    return std::make_unique<ValidateStructureCommand>(
        ctx.config, 
        ctx.config.export_path.value_or("./"),
        ctx.file_system,
        ctx.user_notifier
    );
});

ValidateStructureCommand::ValidateStructureCommand(
    const AppConfig& config, 
    const std::filesystem::path& output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier)
    : app_config_(config), 
      output_root_(output_root), 
      fs_(std::move(fs)),
      notifier_(std::move(notifier)) {}

std::vector<ArgDef> ValidateStructureCommand::get_definitions() const {
    return {
        {"path", ArgType::Positional, {}, "Source directory or file path", true, "", 0}
    };
}

std::string ValidateStructureCommand::get_help() const {
    return "Validates the syntax and structure of source TXT files (Read-only).";
}

void ValidateStructureCommand::execute(const CommandParser& parser) {
    auto args = CommandValidator::validate(parser, get_definitions());

    AppOptions options;
    options.input_path = args.get("path");
    
    options.validate_structure = true;
    options.convert = false;
    options.validate_logic = false;
    options.save_processed_output = false;

    // [修复] 使用 PipelineFactory 和 Context 手动构建流程
    // 1. 准备上下文
    core::pipeline::PipelineContext context(app_config_, output_root_, fs_, notifier_);
    context.config.input_root = options.input_path;
    
    // 2. 通过工厂创建流水线
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(options, app_config_);

    // 3. 执行
    pipeline->run(std::move(context));
}