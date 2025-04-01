
#NOTE: Original build scripts referred to this as r37_llk, so the assigned names
#      will reflect that.

set(WHB0_R37_LLK_SOURCES
    syntax_math.cpp
    syntax_pack.cpp
    syntax_sfpu.cpp
    syntax_unpack.cpp
)

set(WHB0_R37_LLK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_R37_LLK_LIB_NAME "${WHB0_LIB_PREFIX}r37_llk" CACHE STRING "" FORCE)

add_library(whb0_r37_llk
    STATIC
    ${WHB0_R37_LLK_SOURCES}
)


target_include_directories(whb0_r37_llk
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/iss"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/common/inc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/llk_lib"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc/defines"
    
    PRIVATE ${WHB0_R37_LLK_INCLUDE_DIR}
)

set_target_properties(whb0_r37_llk PROPERTIES OUTPUT_NAME ${WHB0_R37_LLK_LIB_NAME})

