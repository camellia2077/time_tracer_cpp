# cmake/TargetSetup.cmake

function(setup_project_target TARGET_NAME)
    # 1. 设置包含目录
    target_include_directories(${TARGET_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )

    # 2. 链接第三方库 (从 Dependencies.cmake 中查找的 tomlplusplus)
    target_link_libraries(${TARGET_NAME} PRIVATE 
        tomlplusplus::tomlplusplus
    )

    # 3. 如果有其他系统库（如 Windows 上的用户库）可以在此添加
    if(WIN32)
        target_link_libraries(${TARGET_NAME} PRIVATE User32)
    endif()
endfunction()