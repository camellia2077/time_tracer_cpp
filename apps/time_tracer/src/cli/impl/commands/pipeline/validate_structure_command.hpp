// cli/impl/commands/pipeline/validate_structure_command.hpp
#ifndef CLI_IMPL_COMMANDS_PIPELINE_VALIDATE_STRUCTURE_COMMAND_HPP_
#define CLI_IMPL_COMMANDS_PIPELINE_VALIDATE_STRUCTURE_COMMAND_HPP_

#include "cli/framework/interfaces/i_command.hpp"
#include "common/config/app_config.hpp"
#include "core/application/interfaces/i_file_system.hpp"
#include "core/application/interfaces/i_user_notifier.hpp" // [新增]
#include <filesystem>
#include <memory> 

class ValidateStructureCommand : public ICommand {
public:
    // [修改] 构造函数增加 notifier
    explicit ValidateStructureCommand(const AppConfig& config, 
                                      const std::filesystem::path& output_root,
                                      std::shared_ptr<core::interfaces::IFileSystem> fs,
                                      std::shared_ptr<core::interfaces::IUserNotifier> notifier);
    
    std::vector<ArgDef> get_definitions() const override;
    std::string get_help() const override;
    void execute(const CommandParser& parser) override;

private:
    const AppConfig& app_config_;
    std::filesystem::path output_root_;
    std::shared_ptr<core::interfaces::IFileSystem> fs_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_; // [新增]
};

#endif