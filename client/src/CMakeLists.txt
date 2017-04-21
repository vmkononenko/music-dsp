set(SOURCES
    lmclient.cpp
)

add_executable(${LMCLIENT_TARGET} ${SOURCES})
add_dependencies(${LMCLIENT_TARGET} ${MUSIC_DSP_TARGET})

find_library(
    LIBSNDFILE
    NAMES sndfile
)

target_link_libraries(${LMCLIENT_TARGET} ${MUSIC_DSP_TARGET} ${LIBSNDFILE})