cmake_minimum_required(VERSION 3.1)
project(hero)

find_package(SDL)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ")

set(SOURCE_FILES
    hero.c
    )

include_directories(${SDL_INCLUDE_DIR})
link_libraries(SDL2)

add_definitions(-std=c11)
add_definitions(-g)
add_definitions(-Wall)
add_definitions(-O0)
add_definitions(-Wunused-variable)

add_executable(hero ${SOURCE_FILES})