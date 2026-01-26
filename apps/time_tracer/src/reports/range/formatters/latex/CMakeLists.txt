add_library(RangeTexFormatter SHARED
    range_tex_formatter.cpp
    range_tex_config.cpp
    range_tex_utils.cpp
)

setup_project_target(RangeTexFormatter)
target_link_libraries(RangeTexFormatter PRIVATE reports_shared)

target_compile_options(RangeTexFormatter PRIVATE -Os -ffunction-sections -fdata-sections)
target_link_options(RangeTexFormatter PRIVATE -s -Wl,--gc-sections)

set_target_properties(RangeTexFormatter PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
)