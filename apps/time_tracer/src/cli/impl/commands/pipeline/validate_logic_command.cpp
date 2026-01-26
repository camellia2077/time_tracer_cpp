// cli/impl/commands/pipeline/validate_logic_command.cpp
#include "cli/impl/commands/pipeline/validate_logic_command.hpp"
#include "cli/framework/core/command_parser.hpp"
#include "cli/framework/core/command_validator.hpp"
#include "cli/impl/utils/arg_utils.hpp"
#include "common/app_options.hpp"
#include "core/application/pipeline_factory.hpp" 
#include "core/application/pipeline/context/pipeline_context.hpp"

#include "cli/framework/core/command_registry.hpp"
#include "cli/impl/app/app_context.hpp"

static CommandRegistrar<AppContext> registrar("validate-logic", [](AppContext& ctx) {
    if (!ctx.serializer || !ctx.log_converter) throw std::runtime_error("Dependencies not initialized");
    return std::make_unique<ValidateLogicCommand>(
        ctx.config, 
        ctx.config.export_path.value_or("./"),
        ctx.file_system,
        ctx.user_notifier,
        ctx.serializer,
        ctx.log_converter // [新增] 传递 converter
    );
});

ValidateLogicCommand::ValidateLogicCommand(
    const AppConfig& config, 
    const std::filesystem::path& output_root,
    std::shared_ptr<core::interfaces::IFileSystem> fs,
    std::shared_ptr<core::interfaces::IUserNotifier> notifier,
    std::shared_ptr<core::interfaces::ILogSerializer> serializer,
    std::shared_ptr<core::interfaces::ILogConverter> converter)
    : app_config_(config), 
      output_root_(output_root), 
      fs_(std::move(fs)), 
      notifier_(std::move(notifier)),
      serializer_(std::move(serializer)),
      converter_(std::move(converter)) {} // [新增]

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
    
    options.validate_structure = false; 
    options.convert = true;             
    options.validate_logic = true;      
    options.save_processed_output = false; 

    if (args.has("date_check")) {
        options.date_check_mode = ArgUtils::parse_date_check_mode(args.get("date_check"));
    } else if (args.has("no_date_check")) {
        options.date_check_mode = DateCheckMode::None;
    }

    core::pipeline::PipelineContext context(app_config_, output_root_, fs_, notifier_);
    context.config.input_root = options.input_path;
    context.config.date_check_mode = options.date_check_mode;
    context.config.save_processed_output = options.save_processed_output;

    // [修改] 传递 converter 给工厂
    auto pipeline = core::pipeline::PipelineFactory::create_ingest_pipeline(options, app_config_, serializer_, converter_);

    pipeline->run(std::move(context));
}