
# NOTE: Original build scripts referred to this directory as r37_ckernel,
#       so the target names assigned are going to reflect that.

set(WHB0_R37_COMMON_SOURCES
    syntax.cpp
)

set(WHB0_R37_COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_R37_COMMON_LIB_NAME "${WHB0_LIB_PREFIX}r37_ckernel" CACHE STRING "" FORCE)

add_library(whb0_r37_ckernel
    STATIC
    ${WHB0_R37_COMMON_SOURCES}
)

target_include_directories(whb0_r37_ckernel
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/iss"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/common/inc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/llk_lib"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc/defines"
    
    PRIVATE ${WHB0_R37_COMMON_INCLUDE_DIR} #technically redundant with common/inc, but put for consistency
)

set_target_properties(whb0_r37_ckernel PROPERTIES OUTPUT_NAME ${WHB0_R37_COMMON_LIB_NAME})
