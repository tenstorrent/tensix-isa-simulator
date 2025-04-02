
add_executable(whb0_demo_sfpi_gen)

set_target_properties(whb0_demo_sfpi_gen
    PROPERTIES
        OUTPUT_NAME "${WHB0_BIN_PREFIX}demo_sfpi_gen"
)

target_sources(whb0_demo_sfpi_gen
    PRIVATE
        main.cpp
        spfi_test.cpp
)

target_include_directories(whb0_demo_sfpi_gen
    PUBLIC
        ${WHB0_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(whb0_demo_sfpi_gen
    whb0::sfpi
)

