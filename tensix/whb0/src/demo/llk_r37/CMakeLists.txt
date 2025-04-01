
set(WHB0_DEMO_LLK_R37_SOURCES
    bcast_h_add_math.cpp
    bcast_h_add_pack.cpp
    bcast_h_add_unpack.cpp
    bcast_hw_add_math.cpp
    bcast_hw_add_pack.cpp
    bcast_hw_add_unpack.cpp
    bcast_w_add_math.cpp
    bcast_w_add_pack.cpp
    bcast_w_add_unpack.cpp
    binary_add_math.cpp
    binary_add_pack.cpp
    binary_add_unpack.cpp
    kernels.cpp
    machine.cpp
    main.cpp
    matmul_math.cpp
    matmul_pack.cpp
    matmul_unpack.cpp
    reduce_h_sum_math.cpp
    reduce_h_sum_pack.cpp
    reduce_h_sum_unpack.cpp
    reduce_hw_sum_math.cpp
    reduce_hw_sum_pack.cpp
    reduce_hw_sum_unpack.cpp
    reduce_w_sum_math.cpp
    reduce_w_sum_pack.cpp
    reduce_w_sum_unpack.cpp
    sfpu_math.cpp
    sfpu_pack.cpp
    sfpu_unpack.cpp
    tilize_math.cpp
    tilize_pack.cpp
    tilize_unpack.cpp
    trisc.cpp
    untilize_math.cpp
    untilize_pack.cpp
    untilize_unpack.cpp
)


set(WHB0_DEMO_LLK_R37_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_DEMO_LLK_R37_BIN_NAME "${WHB0_BIN_PREFIX}demo_llk_r37" CACHE STRING "" FORCE)

add_executable(whb0_demo_llk_r37
    ${WHB0_DEMO_LLK_R37_SOURCES}
)

target_include_directories(whb0_demo_llk_r37
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC ${WHB0_R37_INCLUDE_DIR}
    PUBLIC "${WHB0_R37_INCLUDE_DIR}"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/iss"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/common/inc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/llk_lib"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc/defines"
    
    PRIVATE ${WHB0_DEMO_LLK_R37_INCLUDE_DIR}
)

target_link_libraries(whb0_demo_llk_r37
    whb0_r37_iss
    whb0_core
    whb0_schedule
    whb0_sfpi
)

set_target_properties(whb0_demo_llk_r37 PROPERTIES OUTPUT_NAME ${WHB0_DEMO_LLK_R37_BIN_NAME})
