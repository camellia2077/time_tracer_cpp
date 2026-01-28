// cli/framework/ansi_colors.hpp
// CLI 框架：ANSI 终端颜色宏定义
#pragma once
#ifndef CLI_FRAMEWORK_ANSI_COLORS_H_
#define CLI_FRAMEWORK_ANSI_COLORS_H_

// =================================================================
//                 ANSI 终端颜色宏定义
// =================================================================
// 通过宏定义提供简单的颜色访问方式。
// 任何需要颜色输出的模块都可以包含此文件。
// =================================================================

#define RED_COLOR    "\033[1;31m"
#define GREEN_COLOR  "\033[1;32m"
#define RESET_COLOR  "\033[0m"

#endif  // CLI_FRAMEWORK_ANSI_COLORS_H_
