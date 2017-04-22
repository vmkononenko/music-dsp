cmake_minimum_required (VERSION 3.1)

project (MusicDsp CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if (NOT CMAKE_LIBRARY_PATH)
	set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

set (LMCLIENT_TARGET lmclient)
set (MUSIC_DSP_TARGET music-dsp)

include_directories(${PROJECT_SOURCE_DIR}/libmusic/include)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

option(WITH_CLIENT ON)
option(WITH_TESTS ON)

if(WITH_CLIENT)
    add_subdirectory(client)
endif()

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(libmusic)
