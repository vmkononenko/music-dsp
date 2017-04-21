cmake_minimum_required (VERSION 3.1)

project (MusicDsp CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set (LMCLIENT_TARGET lmclient)
set (MUSIC_DSP_TARGET music-dsp)

include_directories(${PROJECT_SOURCE_DIR}/libmusic/include)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

option(WITH_CLIENT ON)
option(WITH_TESTS ON)

if(WITH_CLIENT)
    add_subdirectory(client)
endif(WITH_CLIENT)

if(WITH_TESTS)
    add_subdirectory(tests)
endif(WITH_TESTS)

add_subdirectory(libmusic)