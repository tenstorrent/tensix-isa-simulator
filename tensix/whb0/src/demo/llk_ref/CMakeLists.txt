
add_executable(whb0_demo_llk_ref)

set_target_properties(whb0_demo_llk_ref
    PROPERTIES
        OUTPUT_NAME "${WHB0_BIN_PREFIX}demo_llk_ref"
)

target_sources(whb0_demo_llk_ref
    PRIVATE
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

target_include_directories(whb0_demo_llk_ref
    PUBLIC
        ${WHB0_INCLUDE_DIR}
        "${WHB0_INCLUDE_DIR}/system"
        ${WHB0_LLK_INCLUDE_DIR}
        "${WHB0_LLK_INCLUDE_DIR}/api"
        "${WHB0_LLK_INCLUDE_DIR}/ref"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(whb0_demo_llk_ref
    whb0::llk::api
    whb0::core
    whb0::schedule
    whb0::sfpi
)

