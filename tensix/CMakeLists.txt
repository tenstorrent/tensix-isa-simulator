cmake_minimum_required(VERSION 3.22...3.30)

project(tensix-isa-simulator
    LANGUAGES NONE
    VERSION 0.0.1
    DESCRIPTION ""
)

option(BUILD_WHB0 "Enable building whb0" ON)

if(BUILD_WHB0)
    add_subdirectory(whb0)
endif()
