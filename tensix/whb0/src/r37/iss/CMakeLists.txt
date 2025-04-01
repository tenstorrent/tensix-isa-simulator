
set(WHB0_R37_ISS_SOURCES
    ckernel.cpp
    ckernel_mem_ptr.cpp
    ckernel_ops.cpp
    ckernel_template.cpp
    ckernel_thread.cpp
    sfpi.cpp
)

set(WHB0_R37_ISS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_R37_ISS_LIB_NAME "${WHB0_LIB_PREFIX}r37_iss" CACHE STRING "" FORCE)

add_library(whb0_r37_iss 
    STATIC
    ${WHB0_R37_ISS_SOURCES}
)

target_include_directories(whb0_r37_iss 
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/iss"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/common/inc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/llk_lib"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc/defines"
    
    PRIVATE ${WHB0_R37_ISS_INCLUDE_DIR} 
)

set_target_properties(whb0_r37_iss PROPERTIES OUTPUT_NAME ${WHB0_R37_ISS_LIB_NAME})
