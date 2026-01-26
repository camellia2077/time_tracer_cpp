// cli/impl/commands/pipeline/validate_logic_command.hpp
#ifndef CLI_IMPL_COMMANDS_PIPELINE_VALIDATE_LOGIC_COMMAND_HPP_
#define CLI_IMPL_COMMANDS_PIPELINE_VALIDATE_LOGIC_COMMAND_HPP_

#include "cli/framework/interfaces/i_command.hpp"
#include "common/config/app_config.hpp"
#include "core/application/interfaces/i_file_system.hpp"
#include "core/application/interfaces/i_user_notifier.hpp"
#include "core/application/interfaces/i_log_serializer.hpp"
#include "core/application/interfaces/i_log_converter.hpp" // [新增]
#include <filesystem>
#include <memory>

class ValidateLogicCommand : public ICommand {
public:
    explicit ValidateLogicCommand(const AppConfig& config, 
                                  const std::filesystem::path& output_root,
                                  std::shared_ptr<core::interfaces::IFileSystem> fs,
                                  std::shared_ptr<core::interfaces::IUserNotifier> notifier,
                                  std::shared_ptr<core::interfaces::ILogSerializer> serializer,
                                  std::shared_ptr<core::interfaces::ILogConverter> converter); // [新增]
    
    std::vector<ArgDef> get_definitions() const override;
    std::string get_help() const override;
    void execute(const CommandParser& parser) override;

private:
    const AppConfig& app_config_;
    std::filesystem::path output_root_;
    std::shared_ptr<core::interfaces::IFileSystem> fs_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
    std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
    std::shared_ptr<core::interfaces::ILogConverter> converter_; // [新增]
};

#endif