// cli/impl/config_validator.hpp
#ifndef CLI_IMPL_CONFIG_VALIDATOR_HPP_
#define CLI_IMPL_CONFIG_VALIDATOR_HPP_

#include "common/config_types.hpp"

namespace Cli::Impl {

    /**
     * @class ConfigValidator
     * @brief 负责校验配置对象的业务逻辑合法性。
     */
    class ConfigValidator {
    public:
        // 如果校验不通过，抛出 std::logic_error
        static void validate(const Config& config);
    };

}

#endif // CLI_IMPL_CONFIG_VALIDATOR_HPP_