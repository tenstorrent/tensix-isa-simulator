
add_library(whb0_schedule STATIC)
add_library(whb0::schedule ALIAS whb0_schedule)

set_target_properties(whb0_schedule
    PROPERTIES
        OUTPUT_NAME "${WHB0_LIB_PREFIX}schedule")

target_sources(whb0_schedule
    PRIVATE
        minicoro.cpp
        minicoro_impl.cpp
        schedule.cpp
)

target_include_directories(whb0_schedule  
    PUBLIC
        ${WHB0_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

