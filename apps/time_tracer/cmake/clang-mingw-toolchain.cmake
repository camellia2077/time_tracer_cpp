set(CMAKE_SYSTEM_NAME Windows)

# 1. 指定 Clang 编译器
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# 2. 关键：指定交叉编译目标架构
set(TARGET_ARCH x86_64-w64-windows-gnu)

# =========================================================
# [核心修复] 自动定位 Ubuntu MinGW Posix 路径
# =========================================================
file(GLOB GCC_DIRS "/usr/lib/gcc/x86_64-w64-mingw32/*-posix")

if(GCC_DIRS)
    # 获取根路径 (例如 /usr/lib/gcc/x86_64-w64-mingw32/13-posix)
    list(GET GCC_DIRS 0 GCC_ROOT)
    message(STATUS "Auto-detected MinGW GCC Posix root: ${GCC_ROOT}")
    
    # 准备头文件路径 (仅用于编译)
    set(CXX_INCLUDES 
        "-isystem ${GCC_ROOT}/include/c++"
        "-isystem ${GCC_ROOT}/include/c++/x86_64-w64-mingw32"
        "-isystem ${GCC_ROOT}/include/c++/backward"
    )
    string(REPLACE ";" " " CXX_INCLUDES_STR "${CXX_INCLUDES}")

    # 准备库路径 (仅用于链接)
    set(LINKER_LIB_PATH "-L${GCC_ROOT}")

    # 3. 设置编译器标志
    set(COMMON_FLAGS "--target=${TARGET_ARCH} -fuse-ld=lld ${CXX_INCLUDES_STR}")
    set(CMAKE_C_FLAGS "${COMMON_FLAGS}" CACHE STRING "C Flags" FORCE)
    set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}" CACHE STRING "CXX Flags" FORCE)

    # 4. [终极修复] 设置链接器标志
    # -static-libgcc -static-libstdc++ : 强制链接静态标准库，防止链接到系统默认的旧版 DLL
    # 这能彻底解决 "undefined symbol" 问题，且生成的 exe 更便携
    set(LINK_FLAGS "--target=${TARGET_ARCH} -fuse-ld=lld ${LINKER_LIB_PATH} -static-libgcc -static-libstdc++")
    
    set(CMAKE_EXE_LINKER_FLAGS    "${LINK_FLAGS}" CACHE STRING "Exe Linker Flags" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS "${LINK_FLAGS}" CACHE STRING "Shared Linker Flags" FORCE)
    set(CMAKE_MODULE_LINKER_FLAGS "${LINK_FLAGS}" CACHE STRING "Module Linker Flags" FORCE)
    
else()
    message(WARNING "Could not find MinGW GCC Posix libraries.")
    set(TOOLCHAIN_FLAGS "--target=${TARGET_ARCH} -fuse-ld=lld")
    set(CMAKE_C_FLAGS "${TOOLCHAIN_FLAGS}" CACHE STRING "C Flags" FORCE)
    set(CMAKE_CXX_FLAGS "${TOOLCHAIN_FLAGS}" CACHE STRING "CXX Flags" FORCE)
endif()

# 基础配置
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)