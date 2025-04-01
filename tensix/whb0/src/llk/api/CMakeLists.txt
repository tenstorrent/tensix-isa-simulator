
set(WHB0_LLK_API_SOURCES
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

set(WHB0_LLK_API_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_LLK_API_LIB_NAME "${WHB0_LIB_PREFIX}llk_api" CACHE STRING "" FORCE)

add_library(whb0_llk_api
    STATIC
    ${WHB0_LLK_API_SOURCES}
)

target_include_directories(whb0_llk_api 
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC ${WHB0_LLK_API_INCLUDE_DIR}
)

set_target_properties(whb0_llk_api PROPERTIES OUTPUT_NAME ${WHB0_LLK_API_LIB_NAME})
