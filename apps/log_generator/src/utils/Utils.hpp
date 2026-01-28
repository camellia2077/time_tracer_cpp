// utils/Utils.hpp
#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <string>

namespace utils {
    
    // 设置控制台以支持UTF-8和颜色代码
    void SetupConsole();

    // 判断是否为闰年
    bool IsLeap(int year);

    // 获取指定月份的天数
    int GetDaysInMonth(int year, int month);
    
} // namespace utils
#endif // UTILS_UTILS_H_