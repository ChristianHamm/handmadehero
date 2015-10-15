cmake_minimum_required(VERSION 3.1)
project(hero)

find_package(SDL2)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ")

set(SOURCE_FILES
    debug_stuff.c
    globals.c
    hero.c
    logic.c
    platform.c
    )

set(LOGIC_SOURCE_FILES
    debug_stuff.c
    logic.c
)

include_directories(${SDL_INCLUDE_DIR})
LINK_DIRECTORIES(/usr/local/lib)
link_libraries(SDL2)

add_definitions(-std=c11)
add_definitions(-g)
add_definitions(-Wall)
add_definitions(-O0)
add_definitions(-Wunused-variable)

add_executable(hero ${SOURCE_FILES})

add_library(herologic  MODULE ${LOGIC_SOURCE_FILES} )
