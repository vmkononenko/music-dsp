include_directories(${CMAKE_BINARY_DIR}/include)

set(PLUGIN_SOURCES
    parachord.cpp
    plugins.cpp
)

add_library(${VAMP_TARGET} SHARED ${PLUGIN_SOURCES})
set_target_properties(${VAMP_TARGET} PROPERTIES PREFIX "")

add_dependencies(${VAMP_TARGET} ${EXT_VAMPSDK_TARGET} ${MUSIC_DSP_TARGET})

target_link_libraries(${VAMP_TARGET} ${MUSIC_DSP_TARGET} vamp-sdk)
