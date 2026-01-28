// validator/txt/facade/text_validator.hpp
#ifndef VALIDATOR_TXT_FACADE_TEXT_VALIDATOR_HPP_
#define VALIDATOR_TXT_FACADE_TEXT_VALIDATOR_HPP_

#include "common/config/models/converter_config_models.hpp"
#include "validator/common/validator_utils.hpp"

#include <memory>
#include <set>
#include <string>

namespace validator {
namespace txt {

class TextValidator {
public:
  // 构造函数接收配置，用于校验关键字是否合法
  explicit TextValidator(const ConverterConfig &config);
  ~TextValidator();

  bool validate(const std::string &filename, const std::string &content,
                std::set<Error> &errors);

private:
  struct PImpl;
  std::unique_ptr<PImpl> pimpl_;
};

} // namespace txt
} // namespace validator

#endif // VALIDATOR_TXT_FACADE_TEXT_VALIDATOR_HPP_
