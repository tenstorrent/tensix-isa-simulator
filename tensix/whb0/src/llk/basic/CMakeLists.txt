
add_library(whb0_llk_basic STATIC)
add_library(whb0::llk::basic ALIAS whb0_llk_basic)

set_target_properties(whb0_llk_basic
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}llk_basic"
)

target_sources(whb0_llk_basic
    PRIVATE
        math_api.cpp
        math_common.cpp
        math_eltwise_binary.cpp
        math_eltwise_unary_datacopy.cpp
        math_eltwise_unary_sfpu.cpp
        math_matmul.cpp
        math_reduce.cpp
        pack_api.cpp
        pack_common.cpp
        pack.cpp
        pack_reduce.cpp
        sfpu.cpp
        unpack_ab.cpp
        unpack_ab_matmul.cpp
        unpack_a.cpp
        unpack_api.cpp
        unpack_common.cpp
        unpack_reduce.cpp
        unpack_tilize.cpp
        unpack_untilize.cpp
)

target_include_directories(whb0_llk_basic
    PUBLIC
        ${WHB0_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

