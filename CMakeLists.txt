cmake_minimum_required(VERSION 3.1)
project(hero)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ")

set(SOURCE_FILES
        debug_stuff.c
        globals.c
        hero.c
        logic.c
        platform.c
        tile.c
        )

set(LOGIC_SOURCE_FILES
        debug_stuff.c
        logic.c
        tile.c)

IF (APPLE)
    #SET(GUI_TYPE MACOSX_BUNDLE)
    FIND_LIBRARY(SDL2_LIBRARY SDL2 sdl)
    INCLUDE_DIRECTORIES(sdl/SDL2.framework/Headers)
    MARK_AS_ADVANCED(SDL2_LIBRARY)
    SET(EXTRA_LIBS ${SDL2_LIBRARY})
ENDIF (APPLE)

add_definitions(-std=c11)
add_definitions(-g)
add_definitions(-Wall)
add_definitions(-O0)
add_definitions(-msse4.2)
add_definitions(-Wno-unused-variable)

add_executable(hero ${SOURCE_FILES})
add_library(herologic MODULE ${LOGIC_SOURCE_FILES})
target_link_libraries(hero ${EXTRA_LIBS})
target_link_libraries(herologic ${EXTRA_LIBS})

