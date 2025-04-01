
set(WHB0_R37_COMPUTE_SOURCES
    syntax_math.cpp
    syntax_pack.cpp
    syntax_unpack.cpp
)

set(WHB0_R37_COMPUTE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_R37_COMPUTE_LIB_NAME "${WHB0_LIB_PREFIX}core" CACHE STRING "" FORCE)

add_library(whb0_r37_compute
    STATIC
    ${WHB0_R37_COMPUTE_SOURCES}
)

target_include_directories(whb0_r37_compute
    PUBLIC ${WHB0_INCLUDE_DIR}
    PUBLIC ${WHB0_R37_INCLUDE_DIR}
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/iss"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/common/inc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/llk_lib"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc"
    PUBLIC "${WHB0_R37_INCLUDE_DIR}/etc/defines"

    PRIVATE ${WHB0_R37_COMPUTE_INCLUDE_DIR}
)
