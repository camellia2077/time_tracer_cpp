// core/domain/model/processing_result.hpp
#ifndef CORE_DOMAIN_MODEL_PROCESSING_RESULT_HPP_
#define CORE_DOMAIN_MODEL_PROCESSING_RESULT_HPP_

struct ProcessingTimings {
    double validation_source_ms = 0.0;
    double conversion_ms = 0.0;
    double validation_output_ms = 0.0;
};

struct ProcessingResult {
    bool success = true;
    ProcessingTimings timings;
};

#endif // CORE_DOMAIN_MODEL_PROCESSING_RESULT_HPP_