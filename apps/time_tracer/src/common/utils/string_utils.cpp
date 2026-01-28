// common/utils/string_utils.cpp
#include "common/utils/string_utils.hpp"
#include <sstream>

std::string Trim(const std::string &str) {
  const std::string kWhitespace = " \n\r\t\f\v";
  size_t first = str.find_first_not_of(kWhitespace);
  if (std::string::npos == first) {
    return "";
  }
  size_t last = str.find_last_not_of(kWhitespace);
  return str.substr(first, (last - first + 1));
}

std::vector<std::string> SplitString(const std::string &str, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream token_stream(str);
  while (std::getline(token_stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}
