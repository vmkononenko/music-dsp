set(SOURCES
    chord_detector_test.cpp
    fft_test.cpp
    helpers_test.cpp
    pitch_calculator_test.cpp
    test_run.cpp
    viterbi_test.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../cute ${SND_HEADERS})

add_executable(${TESTS_TARGET} ${SOURCES})
add_dependencies(${TESTS_TARGET} ${MUSIC_DSP_TARGET})

target_link_libraries(${TESTS_TARGET} ${MUSIC_DSP_TARGET} ${LIBSNDFILE})
