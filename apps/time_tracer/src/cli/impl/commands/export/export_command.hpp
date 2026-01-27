// cli/impl/commands/export/export_command.hpp
#ifndef CLI_IMPL_COMMANDS_EXPORT_EXPORT_COMMAND_HPP_
#define CLI_IMPL_COMMANDS_EXPORT_EXPORT_COMMAND_HPP_

#include "cli/framework/interfaces/i_command.hpp"
#include "core/application/interfaces/i_report_handler.hpp" 
#include <memory> // [新增]

class ExportCommand : public ICommand { 
public: 
    // [修改] 接收 shared_ptr
    explicit ExportCommand(std::shared_ptr<IReportHandler> report_handler);

    std::vector<ArgDef> get_definitions() const override;
    std::string get_help() const override;
    void execute(const CommandParser& parser) override;

private:
    // [修改] 存储 shared_ptr
    std::shared_ptr<IReportHandler> report_handler_;
};

#endif