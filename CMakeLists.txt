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
set (TESTS_TARGET tests)

include_directories(${PROJECT_SOURCE_DIR}/libmusic/include)

if (DEFINED ANDROID_ABI)
    if (${ANDROID_ABI} STREQUAL "x86_64")
        include_directories(${ANDROID_SYSROOT}/usr/include/x86_64-linux-android)
    elseif (${ANDROID_ABI} STREQUAL "x86")
        include_directories(${ANDROID_SYSROOT}/usr/include/i686-linux-android)
    elseif (${ANDROID_ABI} STREQUAL "arm64-v8a")
        include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
    elseif (${ANDROID_ABI} STREQUAL "armeabi-v7a")
        include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
    endif()
endif()

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

option(WITH_CLIENT ON)
option(WITH_TESTS ON)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")

if(WITH_CLIENT)
    add_subdirectory(client)
endif()

add_subdirectory(libmusic)
