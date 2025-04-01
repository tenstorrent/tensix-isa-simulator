
set(WHB0_DEMO_LLK_BASIC_SOURCES
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

set(WHB0_DEMO_LLK_BASIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_DEMO_LLK_BASIC_BIN_NAME "${WHB0_BIN_PREFIX}demo_llk_basic" CACHE STRING "" FORCE)

add_executable(whb0_demo_llk_basic
    ${WHB0_DEMO_LLK_BASIC_SOURCES}
)

target_include_directories(whb0_demo_llk_basic
    PUBLIC ${WHB0_INCLUDE_DIR}
    PRIVATE ${WHB0_DEMO_LLK_BASIC_INCLUDE_DIR}
)

target_link_libraries(whb0_demo_llk_basic
    whb0_llk_basic
    whb0_core
    whb0_schedule
)

set_target_properties(whb0_demo_llk_basic PROPERTIES OUTPUT_NAME ${WHB0_DEMO_LLK_BASIC_BIN_NAME})
