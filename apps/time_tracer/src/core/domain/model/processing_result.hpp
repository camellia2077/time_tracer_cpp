// core/domain/model/processing_result.hpp
#ifndef CORE_DOMAIN_MODEL_PROCESSING_RESULT_HPP_
#define CORE_DOMAIN_MODEL_PROCESSING_RESULT_HPP_

struct ProcessingTimings {
  double validation_source_ms_ = 0.0;
  double conversion_ms_ = 0.0;
  double validation_output_ms_ = 0.0;
};

struct ProcessingResult {
  bool success_ = true;
  ProcessingTimings timings_;
};

#endif // CORE_DOMAIN_MODEL_PROCESSING_RESULT_HPP_
