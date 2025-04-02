
add_library(whb0_sfpi STATIC)
add_library(whb0::sfpi ALIAS whb0_sfpi)

set_target_properties(whb0_sfpi
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}sfpi"
)

target_sources(whb0_sfpi
    PRIVATE
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

target_include_directories(whb0_sfpi
    PUBLIC
        ${WHB0_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

