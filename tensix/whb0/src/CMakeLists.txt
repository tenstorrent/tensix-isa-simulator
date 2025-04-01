
set(WHB0_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "" FORCE)

# Library directories
add_subdirectory(core)
add_subdirectory(llk)
add_subdirectory(minicoro)
add_subdirectory(r37)
add_subdirectory(schedule)
add_subdirectory(sfpi)
add_subdirectory(system)

# Binary Executable directories
add_subdirectory(demo)
