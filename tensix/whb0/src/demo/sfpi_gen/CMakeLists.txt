
set(WHB0_DEMO_SFPI_GEN_SOURCES
    main.cpp
    spfi_test.cpp
)

set(WHB0_DEMO_SFPI_GEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_DEMO_SFPI_GEN_BIN_NAME "${WHB0_BIN_PREFIX}demo_sfpi_gen" CACHE STRING "" FORCE)

add_executable(whb0_demo_sfpi_gen
    ${WHB0_DEMO_SFPI_GEN_SOURCES}
)

target_include_directories(whb0_demo_sfpi_gen
    PUBLIC ${WHB0_INCLUDE_DIR}
    PRIVATE ${WHB0_DEMO_SFPI_GEN_INCLUDE_DIR}
)

target_link_libraries(whb0_demo_sfpi_gen
    whb0_sfpi
)

set_target_properties(whb0_demo_sfpi_gen PROPERTIES OUTPUT_NAME ${WHB0_DEMO_SFPI_GEN_BIN_NAME})
