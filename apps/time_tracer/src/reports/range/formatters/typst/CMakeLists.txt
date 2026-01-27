add_library(RangeTypFormatter SHARED
    range_typ_formatter.cpp
)

setup_project_target(RangeTypFormatter)
target_link_libraries(RangeTypFormatter PRIVATE reports_shared)

target_compile_options(RangeTypFormatter PRIVATE -Os -ffunction-sections -fdata-sections)
target_link_options(RangeTypFormatter PRIVATE -s -Wl,--gc-sections)

set_target_properties(RangeTypFormatter PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_OUTPUT_DIR}"
)