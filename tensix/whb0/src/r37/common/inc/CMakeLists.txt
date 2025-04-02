
# NOTE: Original build scripts referred to this directory as r37_ckernel,
#       so the target names assigned are going to reflect that.

add_library(whb0_r37_ckernel STATIC)
add_library(whb0::r37::ckernel ALIAS whb0_r37_ckernel)

set_target_properties(whb0_r37_ckernel
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}r37_ckernel"
)

target_sources(whb0_r37_ckernel
    PRIVATE
        syntax.cpp
)

target_include_directories(whb0_r37_ckernel
    PUBLIC 
        ${WHB0_INCLUDE_DIR}
        "${WHB0_R37_INCLUDE_DIR}/iss"
        "${WHB0_R37_INCLUDE_DIR}/common/inc"
        "${WHB0_R37_INCLUDE_DIR}/llk_lib"
        "${WHB0_R37_INCLUDE_DIR}/etc"
        "${WHB0_R37_INCLUDE_DIR}/etc/defines"
    
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR} 
)

