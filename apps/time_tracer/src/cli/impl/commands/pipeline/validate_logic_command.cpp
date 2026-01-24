// cli/impl/commands/pipeline/validate_logic_command.cpp
#include "cli/impl/commands/pipeline/validate_logic_command.hpp"
#include "cli/framework/core/command_parser.hpp"
#include "cli/framework/core/command_validator.hpp"
#include "cli/impl/utils/arg_utils.hpp"
#include "common/app_options.hpp"
// [修改] 引入新的 Factory 和 Context，替代 PipelineManager
#include "core/application/pipeline_factory.hpp" 
#include "core/application/pipeline/context/pipeline_context.hpp"

#include "cli/framework/core/command_registry.hpp"
#include "cli/impl/app/app_context.hpp"

static CommandRegistrar<AppContext> registrar("validate-logic", [](AppContext& ctx) {
    return std::make_unique<ValidateLogicCommand>(
        ctx.config, 
        ctx.config.export_path.value_or("./"),
        ctx.file_system,
        ctx.user_notifier
    );
});

ValidateLogicCommand::ValidateLogicCommand(
    const AppConfig& config, 
    const std::filesystem::path& output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier)
    : app_config_(config), 
      output_root_(output_root), 
      fs_(std::move(fs)), 
      notifier_(std::move(notifier)) {}

std::vector<ArgDef> ValidateLogicCommand::get_definitions() const {
    return {
        {"path", ArgType::Positional, {}, "Source directory path", true, "", 0},
        {"date_check", ArgType::Option, {"--date-check"}, "Date check mode (none/continuity/full)", false, "none"},
        {"no_date_check", ArgType::Flag, {"--no-date-check"}, "Disable date check", false, ""}
    };
}

std::string ValidateLogicCommand::get_help() const {
    return "Validates business logic (e.g., date continuity, sleep cycles). Requires conversion but won't save output.";
}

void ValidateLogicCommand::execute(const CommandParser& parser) {
    auto args = CommandValidator::validate(parser, get_definitions());

    AppOptions options;
    options.input_path = args.get("path");
    
    // 逻辑验证需要先转换数据，但不保存，也不需要重新验证结构（假设结构已通过或不关心）
    options.validate_structure = false; 
    options.convert = true;             
    options.validate_logic = true;      
    options.save_processed_output = false; 

    if (args.has("date_check")) {
        options.date_check_mode = ArgUtils::parse_date_check_mode(args.get("date_check"));
    } else if (args.has("no_date_check")) {
        options.date_check_mode = DateCheckMode::None;
    }

    // [修复] 使用 PipelineFactory 和 Context 手动构建流程
    // 1. 准备上下文
    core::pipeline::PipelineContext context(app_config_, output_root_, fs_, notifier_);
    context.config.input_root = options.input_path;
    context.config.date_check_mode = options.date_check_mode;
    context.config.save_processed_output = options.save_processed_output;

    // 2. 通过工厂创建流水线
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(options, app_config_);

    // 3. 执行
    pipeline->run(std::move(context));
}