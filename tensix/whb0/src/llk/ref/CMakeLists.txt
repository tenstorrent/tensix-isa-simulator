
add_library(whb0_llk_ref STATIC)
add_library(whb0::llk::ref ALIAS whb0_llk_ref)

set_target_properties(whb0_llk_ref
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}llk_ref"
)

target_sources(whb0_llk_ref 
    PRIVATE
        syntax_math.cpp
        syntax_pack.cpp
        syntax_sfpu.cpp
        syntax_unpack.cpp
)

target_include_directories(whb0_llk_ref
    PUBLIC 
        ${WHB0_INCLUDE_DIR}
        "${WHB0_INCLUDE_DIR}/system"
        "${WHB0_LLK_INCLUDE_DIR}/api"
        "${WHB0_LLK_INCLUDE_DIR}/ref"
    
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

