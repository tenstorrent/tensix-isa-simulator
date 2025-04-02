
add_library(whb0_llk_api STATIC)
add_library(whb0::llk::api ALIAS whb0_llk_api)

set_target_properties(whb0_llk_api
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}llk_api"
)

target_sources(whb0_llk_api
    PRIVATE
        ckernel_addrmod.cpp
        ckernel.cpp
        ckernel_mem_ptr.cpp
        ckernel_ops.cpp
        ckernel_sfpi.cpp
        ckernel_template.cpp
        ckernel_thread.cpp
        cmath_common.cpp
        cpack_common.cpp
        cunpack_common.cpp
)

target_include_directories(whb0_llk_api
    PUBLIC
        ${WHB0_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

