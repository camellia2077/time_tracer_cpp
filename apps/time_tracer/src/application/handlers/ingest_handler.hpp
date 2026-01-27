// application/handlers/ingest_handler.hpp
// 入库处理�?- Command Handler 模式
// 内部调用现有 Pipeline 实现，支持渐进式迁移
#ifndef APPLICATION_HANDLERS_INGEST_HANDLER_HPP_
#define APPLICATION_HANDLERS_INGEST_HANDLER_HPP_

#include <memory>
#include <filesystem>
#include "application/commands/ingest_command.hpp"
#include "application/interfaces/i_file_system.hpp"
#include "application/interfaces/i_user_notifier.hpp"
#include "application/interfaces/i_log_converter.hpp"
#include "application/interfaces/i_log_serializer.hpp"
#include "common/config/app_config.hpp"

namespace fs = std::filesystem;

namespace core::handlers {

class IngestHandler {
public:
    IngestHandler(
        const AppConfig& config,
        const fs::path& output_root,
        std::shared_ptr<core::interfaces::IFileSystem> file_system,
        std::shared_ptr<core::interfaces::IUserNotifier> notifier,
        std::shared_ptr<core::interfaces::ILogConverter> converter,
        std::shared_ptr<core::interfaces::ILogSerializer> serializer
    );
    
    ~IngestHandler();

    // 执行入库命令 - 内部调用现有 Pipeline
    core::commands::IngestResult handle(const core::commands::IngestCommand& command);

private:
    const AppConfig& config_;
    fs::path output_root_;
    std::shared_ptr<core::interfaces::IFileSystem> file_system_;
    std::shared_ptr<core::interfaces::IUserNotifier> notifier_;
    std::shared_ptr<core::interfaces::ILogConverter> converter_;
    std::shared_ptr<core::interfaces::ILogSerializer> serializer_;
};

} // namespace core::handlers

#endif // APPLICATION_HANDLERS_INGEST_HANDLER_HPP_
