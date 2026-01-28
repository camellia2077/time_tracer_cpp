# src/CMakeLists.txt
# 升级最低版本以更好地支持 C++23 和现代 CMake 特性
cmake_minimum_required(VERSION 3.25)

project(TimeTracer VERSION "0.5.0" LANGUAGES CXX)

# 强制 CMake 的命令行输出使用 UTF-8 编码
set(CMAKE_CLI_FORCE_UTF8 ON)

# 所有源文件都必须明确写出它们的扩展名
cmake_policy(SET CMP0115 NEW)

# --- Find External Dependencies ---
find_package(SQLite3 REQUIRED)


# ==================== [关键修改: 顺序调整与修复] ====================
# 1. 先设置输出目录！确保 yyjson.dll 生成在 bin/ 下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 2. 配置 yyjson 选项
set(BUILD_SHARED_LIBS ON) 
set(YYJSON_INSTALL OFF CACHE BOOL "Disable yyjson default install" FORCE)

include(FetchContent)

# --- 声明 yyjson ---
FetchContent_Declare(
  yyjson
  GIT_REPOSITORY https://github.com/ibireme/yyjson.git
  GIT_TAG        master
)

# --- 声明 tomlplusplus ---
FetchContent_Declare(
  tomlplusplus
  GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
  GIT_TAG        v3.4.0 
  # 建议使用稳定版本
)

# 一次性使两者可用
FetchContent_MakeAvailable(yyjson tomlplusplus)

# 恢复默认构建方式为静态
set(BUILD_SHARED_LIBS OFF)

# ====================================================================

# --- 1. 项目范围的设置 ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# [新增] 添加开关：是否为可执行文件编译图标资源（默认为 ON）关闭 OFF
option(ENABLE_APP_ICON "Enable application icon for Windows executables" OFF)

set(PLUGIN_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins")

# 设置编译器启动器（如ccache）
find_program(CCACHE_EXECUTABLE ccache)
if(CCACHE_EXECUTABLE)
    message(STATUS "ccache found, enabling compiler launcher.")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_EXECUTABLE}")
endif()

# --- 2. 包含模块 ---
# 将模块路径添加到CMake的搜索路径中
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# 1. 基础工具
include(TargetSetup)

# 2. 变量定义 (纯数据)
include(SourceFileCollection)

# 3. [新增] 库目标定义 (逻辑)
include(AddReportsLibrary)      # 定义 reports_shared (Shared, 接口与插件公用)
include(AddReportsDataLibrary)  # 定义 reports_data (Static, 数据层依赖隔离)

# 4. 编译标志
include(BuildTypeFlags)

# --- 3. 定义可执行文件目标 ---
add_executable(time_tracker_cli src/main_cli.cpp
    ${COMMON_SOURCES}
    ${SERIALIZER_SOURCES}
    ${VALIDATOR_SOURCES}
    ${BOOTSTRAP_SOURCES}
    ${CONFIG_SOURCES}
    ${IMPORTER_SOURCES}
    ${REPORTS_SOURCES}
    ${CONVERTER_SOURCES}
    ${IO_SOURCES}
    ${APPLICATION_SOURCES}
    ${CORE_SOURCES}
    ${CLI_SOURCES}
)

# 链接到新的共享库
target_link_libraries(time_tracker_cli PRIVATE 
    reports_shared
    reports_data
)

# 添加子目录以构建所有日报格式化器的 DLL
add_subdirectory(src/reports/presentation/daily/formatters/markdown)
add_subdirectory(src/reports/presentation/daily/formatters/typst)
add_subdirectory(src/reports/presentation/daily/formatters/latex)
add_subdirectory(src/reports/presentation/range/formatters/markdown)
add_subdirectory(src/reports/presentation/range/formatters/typst)
add_subdirectory(src/reports/presentation/range/formatters/latex)

# 定义所有插件目标的列表
set(FORMATTER_PLUGINS
    DayMdFormatter DayTypFormatter DayTexFormatter
    RangeMdFormatter RangeTypFormatter RangeTexFormatter
)

# 统一修改插件属性
foreach(PLUGIN ${FORMATTER_PLUGINS})
    set_target_properties(${PLUGIN} PROPERTIES
        # 1. [关键] 去掉 MinGW 自动添加的 'lib' 前缀
        PREFIX ""
        
        # 2. [可选] 自动输出到 bin/plugins 目录，省去手动移动文件的麻烦
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/plugins"
    )
endforeach()

# 创建一个包含所有可执行文件目标的列表
set(ALL_TARGETS time_tracker_cli)

# --- 4. 为每个目标应用通用配置 ---
foreach(TARGET_NAME ${ALL_TARGETS})
  setup_project_target(${TARGET_NAME})
endforeach()

# 在每次构建后，无论是否为安装包，都将配置文件复制到输出目录
# 这样可以直接从 build/bin 目录运行程序
foreach(TARGET_NAME ${ALL_TARGETS})
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/config"
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/config"
        COMMENT "Copying config folder for ${TARGET_NAME}"
    )
endforeach()

# --- 5. 包含特定功能的模块 ---
include(Win32PostBuildCopy) # 包含并执行DLL复制逻辑 
include(Packaging)          # 包含并执行打包逻辑 

# --- 6. 结束语 ---
message(STATUS "CMake configuration finished. Targets are ready to be built.")