// src/validator/json/facade/JsonValidator.hpp
#ifndef VALIDATOR_JSON_FACADE_JSON_VALIDATOR_HPP_
#define VALIDATOR_JSON_FACADE_JSON_VALIDATOR_HPP_

#include "validator/common/ValidatorUtils.hpp"
#include <string>
#include <set>

// [修改] 移除 nlohmann/json，不需要暴露 yyjson 头文件给外部（如果只在 cpp 使用）
// 如果其他地方没有引用 yyjson.h，这里甚至可以不 include，
// 但为了保持一致性或如果以后需要传参 yyjson_doc，可以 include "yyjson.h"
// 这里我们接收 std::string，所以头文件很干净。

namespace validator {
namespace json {

class JsonValidator {
public:
    explicit JsonValidator(DateCheckMode date_check_mode = DateCheckMode::None);

    /**
     * @brief 验证 JSON 内容
     * @param filename 文件名（用于错误报告）
     * @param json_content JSON 原始内容字符串
     * @param errors 错误集合
     * @return 是否验证通过
     */
    bool validate(const std::string& filename, const std::string& json_content, std::set<Error>& errors);

private:
    DateCheckMode date_check_mode_;
};

} // namespace json
} // namespace validator

#endif // VALIDATOR_JSON_FACADE_JSON_VALIDATOR_HPP_