// converter/convert/core/stats_rules.hpp
#ifndef CONVERTER_CONVERT_CORE_STATS_RULES_HPP_
#define CONVERTER_CONVERT_CORE_STATS_RULES_HPP_

#include "core/domain/model/daily_log.hpp"
#include <initializer_list>

struct StatsRule {
  const char *match_path;
  int ActivityStats::*member;
};

namespace StatsRules {
constexpr StatsRule rules[] = {
    {"study", &ActivityStats::study_time_},
    {"exercise", &ActivityStats::total_exercise_time_},
    {"exercise_cardio", &ActivityStats::cardio_time_},
    {"exercise_anaerobic", &ActivityStats::anaerobic_time_},
    {"routine_grooming", &ActivityStats::grooming_time_},
    {"routine_toilet", &ActivityStats::toilet_time_},
    {"recreation_game", &ActivityStats::gaming_time_},
    {"recreation", &ActivityStats::recreation_time_},
    {"recreation_zhihu", &ActivityStats::recreation_zhihu_time_},
    {"recreation_bilibili", &ActivityStats::recreation_bilibili_time_},
    {"recreation_douyin", &ActivityStats::recreation_douyin_time_}};
}

#endif // CONVERTER_CONVERT_CORE_STATS_RULES_HPP_
