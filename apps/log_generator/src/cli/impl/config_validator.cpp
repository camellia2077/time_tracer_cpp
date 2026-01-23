// cli/impl/config_validator.cpp
#include "cli/impl/config_validator.hpp"
#include <stdexcept>

namespace Cli::Impl {

    void ConfigValidator::validate(const Config& config) {
        if (config.start_year <= 0 || config.end_year <= 0) {
            throw std::logic_error("Years must be positive.");
        }
        if (config.end_year < config.start_year) {
            throw std::logic_error("End year cannot be earlier than start year.");
        }
        if (config.items_per_day < 2) {
            throw std::logic_error("Items per day must be >= 2.");
        }
    }

}