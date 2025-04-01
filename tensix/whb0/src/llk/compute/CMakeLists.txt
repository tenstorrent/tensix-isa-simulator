
set(WBH0_LLK_COMPUTE_SOURCES
    syntax_math.cpp
    syntax_pack.cpp
    syntax_unpack.cpp
)

set(WHB0_LLK_COMPUTE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_LLK_COMPUTE_LIB_NAME "${WHB0_LIB_PREFIX}llk_compute" CACHE STRING "" FORCE)

add_library(whb0_llk_compute
    STATIC
    ${WBH0_LLK_COMPUTE_SOURCES}
)

target_include_directories(whb0_llk_compute
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC "${WHB0_INCLUDE_DIR}/system"
    PUBLIC "${WHB0_INCLUDE_DIR}/llk"
    PUBLIC "${WHB0_INCLUDE_DIR}/llk/api"
    PUBLIC "${WHB0_INCLUDE_DIR}/llk/ref"
    
    PRIVATE ${WHB0_LLK_COMPUTE_INCLUDE_DIR}
)

set_target_properties(whb0_llk_compute PROPERTIES OUTPUT_NAME ${WHB0_LLK_COMPUTE_LIB_NAME})
