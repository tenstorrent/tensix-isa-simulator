
set(WHB0_CORE_SOURCES
    addrmod.cpp
    cfg_api.cpp
    cfg.cpp
    cfg_reg.cpp
    math.cpp
    memory.cpp
    pack.cpp
    sfpu.cpp
    sync.cpp
    tdma.cpp
    tensix.cpp
    thread.cpp
    unpack.cpp
)

set(WHB0_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_CORE_LIB_NAME "${WHB0_LIB_PREFIX}core" CACHE STRING "" FORCE)

add_library(whb0_core 
    STATIC
    ${WHB0_CORE_SOURCES}
)

target_include_directories(whb0_core 
    PUBLIC ${WHB0_INCLUDE_DIR}
    PRIVATE ${WHB0_CORE_INCLUDE_DIR} 
)

set_target_properties(whb0_core PROPERTIES OUTPUT_NAME ${WHB0_CORE_LIB_NAME})

