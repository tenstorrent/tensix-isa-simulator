
add_library(whb0_llk_compute STATIC)
add_library(whb0::llk::compute ALIAS whb0_llk_compute)

set_target_properties(whb0_llk_compute
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}llk_compute"
)

target_sources(whb0_llk_compute
    PRIVATE
        syntax_math.cpp
        syntax_pack.cpp
        syntax_unpack.cpp
)

target_include_directories(whb0_llk_compute
    PUBLIC
        ${WHB0_INCLUDE_DIR}
        "${WHB0_INCLUDE_DIR}/system"
        "${WHB0_LLK_INCLUDE_DIR}"
        "${WHB0_LLK_INCLUDE_DIR}/api"
        "${WHB0_LLK_INCLUDE_DIR}/ref"

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

