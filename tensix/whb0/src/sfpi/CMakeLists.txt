
set(WHB0_SFPI_SOURCES
    code.cpp
    code_expand.cpp
    code_gen.cpp
    code_reg.cpp
    graph.cpp
    graph_fmt.cpp
    hw_dump.cpp
    ir_cc.cpp
    ir.cpp
    ir_expand.cpp
    rvtt.cpp
)

set(WHB0_SFPI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_SFPI_LIB_NAME "${WHB0_LIB_PREFIX}sfpi" CACHE STRING "" FORCE)

add_library(whb0_sfpi
    STATIC
    ${WHB0_SFPI_SOURCES}
)

target_include_directories(whb0_sfpi
    PUBLIC ${WHB0_INCLUDE_DIR}
    PRIVATE ${WHB0_SFPI_INCLUDE_DIR}
)

set_target_properties(whb0_sfpi PROPERTIES OUTPUT_NAME ${WHB0_SFPI_LIB_NAME})
