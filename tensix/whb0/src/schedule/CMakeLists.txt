
set(WBH0_SCHEDULE_SOURCES
    minicoro.cpp
    minicoro_impl.cpp
    schedule.cpp
)

set(WHB0_SCHEDULE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)
set(WHB0_SCHEDULE_LIB_NAME "${WHB0_LIB_PREFIX}schedule" CACHE STRING "" FORCE)

add_library(whb0_schedule  
    STATIC
    ${WBH0_SCHEDULE_SOURCES}
)

target_include_directories(whb0_schedule  
    PUBLIC ${WHB0_INCLUDE_DIR}
    PRIVATE ${WHB0_SCHEDULE_INCLUDE_DIR}
)

set_target_properties(whb0_schedule PROPERTIES OUTPUT_NAME ${WHB0_SCHEDULE_LIB_NAME})
