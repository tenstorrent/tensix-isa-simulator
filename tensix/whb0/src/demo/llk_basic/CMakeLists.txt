
add_executable(whb0_demo_llk_basic)

set_target_properties(whb0_demo_llk_basic
    PROPERTIES
        OUTPUT_NAME "${WHB0_BIN_PREFIX}demo_llk_basic"
)

target_sources(whb0_demo_llk_basic
    PRIVATE
        kernels.cpp
        machine.cpp
        main.cpp
        sfpu_test10.cpp
        sfpu_test11.cpp
        sfpu_test12.cpp
        sfpu_test13.cpp
        sfpu_test14.cpp
        sfpu_test15.cpp
        sfpu_test16.cpp
        sfpu_test17.cpp
        sfpu_test3.cpp
        sfpu_test4.cpp
        sfpu_test5.cpp
        sfpu_test6.cpp
        sfpu_test7.cpp
        sfpu_test8.cpp
        sfpu_test9.cpp
        sfpu_test.cpp
        sfpu_wrap.cpp
)

target_include_directories(whb0_demo_llk_basic
    PUBLIC
        ${WHB0_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(whb0_demo_llk_basic
    whb0::llk::basic
    whb0::core
    whb0::schedule
)

