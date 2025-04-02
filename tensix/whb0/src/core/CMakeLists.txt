add_library(whb0_core STATIC)
add_library(whb0::core ALIAS whb0_core)

set_target_properties(whb0_core
  PROPERTIES
    OUTPUT_NAME "${WHB0_LIB_PREFIX}core"
)

target_sources(whb0_core
  PRIVATE
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

target_include_directories(whb0_core 
  PUBLIC
    ${WHB0_INCLUDE_DIR}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
