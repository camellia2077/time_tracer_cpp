// cli/impl/config_builder.hpp
#ifndef CLI_IMPL_CONFIG_BUILDER_HPP_
#define CLI_IMPL_CONFIG_BUILDER_HPP_

#include "common/config_types.hpp"
#include "cli/framework/parsed_args.hpp"

namespace Cli::Impl {

    /**
     * @class ConfigBuilder
     * @brief 负责将 CLI 解析结果转换为内部配置对象。
     */
    class ConfigBuilder {
    public:
        // 从解析参数构建 Config，如果参数逻辑错误则抛出异常
        static Config build(const Framework::ParsedArgs& args);
    };

}

#endif // CLI_IMPL_CONFIG_BUILDER_HPP_