# CMake 构建配置

本目录包含 TimeTracer 项目的 CMake 构建模块。

## 文件说明

| 文件 | 职责 |
|------|------|
| `SourceFileCollection.cmake` | **源文件列表** - 定义所有源文件变量 (CORE_SOURCES, CONFIG_SOURCES 等) |
| `TargetSetup.cmake` | **目标配置** - 创建可执行文件和库目标 |
| `BuildTypeFlags.cmake` | **编译选项** - Release/Debug 编译器标志 |
| `Packaging.cmake` | **打包配置** - CPack 安装和分发设置 |
| `AddReportsLibrary.cmake` | **报表库** - 创建 reports_*.dll 动态库 |
| `AddReportsDataLibrary.cmake` | **数据库** - 创建 reports_data.dll |
| `Win32PostBuildCopy.cmake` | **后处理** - Windows 平台 DLL 复制 |
| `clang-mingw-toolchain.cmake` | **工具链** - Clang + MinGW 交叉编译 |
| `mingw-toolchain.cmake` | **工具链** - MinGW 编译配置 |

## 模块依赖

```
CMakeLists.txt (root)
    │
    ├─► SourceFileCollection.cmake   # 定义源文件
    ├─► BuildTypeFlags.cmake         # 编译选项
    ├─► TargetSetup.cmake            # 创建目标
    │       └─► AddReportsLibrary.cmake
    │       └─► AddReportsDataLibrary.cmake
    ├─► Win32PostBuildCopy.cmake     # 后处理
    └─► Packaging.cmake              # 打包
```

## 添加新源文件

编辑 `SourceFileCollection.cmake` 中对应的变量：

```cmake
# Core 层
set(CORE_SOURCES
    src/core/application/handlers/new_handler.cpp  # 添加这里
)

# Config 层
set(CONFIG_SOURCES
    src/config/new_file.cpp  # 添加这里
)
```
