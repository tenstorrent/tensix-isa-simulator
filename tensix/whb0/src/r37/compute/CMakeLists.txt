
add_library(whb0_r37_compute STATIC)
add_library(whb0::r37::compute ALIAS whb0_r37_compute)

set_target_properties(whb0_r37_compute
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}r37_compute"
)

target_sources(whb0_r37_compute
    PRIVATE
        syntax_math.cpp
        syntax_pack.cpp
        syntax_unpack.cpp
)

target_include_directories(whb0_r37_compute
    PUBLIC
        ${WHB0_INCLUDE_DIR}
        ${WHB0_R37_INCLUDE_DIR}
        "${WHB0_R37_INCLUDE_DIR}/iss"
        "${WHB0_R37_INCLUDE_DIR}/common/inc"
        "${WHB0_R37_INCLUDE_DIR}/llk_lib"
        "${WHB0_R37_INCLUDE_DIR}/etc"
        "${WHB0_R37_INCLUDE_DIR}/etc/defines"

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
