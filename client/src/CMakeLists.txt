include_directories(${SND_HEADERS})
add_executable(${LMCLIENT_TARGET} lmclient.cpp)
add_executable(${LMCSR_TARGET} lmcsr.cpp)
add_dependencies(${LMCLIENT_TARGET} ${MUSIC_DSP_TARGET})
add_dependencies(${LMCSR_TARGET} ${MUSIC_DSP_TARGET})

target_link_libraries(${LMCLIENT_TARGET} ${MUSIC_DSP_TARGET} ${LIBSNDFILE})
target_link_libraries(${LMCSR_TARGET} ${MUSIC_DSP_TARGET} ${LIBSNDFILE})
