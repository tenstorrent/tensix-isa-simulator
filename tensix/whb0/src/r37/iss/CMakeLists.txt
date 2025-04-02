
add_library(whb0_r37_iss STATIC)
add_library(whb0::r37::iss ALIAS whb0_r37_iss)

set_target_properties(whb0_r37_iss
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}r37_iss"
)

target_sources(whb0_r37_iss
    PRIVATE
        ckernel.cpp
        ckernel_mem_ptr.cpp
        ckernel_ops.cpp
        ckernel_template.cpp
        ckernel_thread.cpp
        sfpi.cpp
)

target_include_directories(whb0_r37_iss
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

