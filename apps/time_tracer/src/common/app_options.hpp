// common/app_options.hpp
#ifndef COMMON_APP_OPTIONS_HPP_
#define COMMON_APP_OPTIONS_HPP_

#include "common/types/date_check_mode.hpp"
#include <filesystem>

namespace fs = std::filesystem;

struct AppOptions {
  fs::path input_path_;
  bool run_all_ = false;
  bool convert_ = false;
  bool validate_structure_ = false;
  bool validate_logic_ = false;
  DateCheckMode date_check_mode_ = DateCheckMode::None;
  bool save_processed_output_ = false;
};

#endif // COMMON_APP_OPTIONS_HPP_
