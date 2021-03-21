set(SOURCES
    lmclient.cpp
)

include_directories(${SND_HEADERS})
add_executable(${LMCLIENT_TARGET} ${SOURCES})
add_dependencies(${LMCLIENT_TARGET} ${MUSIC_DSP_TARGET})

target_link_libraries(${LMCLIENT_TARGET} ${MUSIC_DSP_TARGET} ${LIBSNDFILE})
