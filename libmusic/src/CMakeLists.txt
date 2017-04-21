set(LIB_SOURCES
    chord_detector.cpp
    chord_tpl_collection.cpp
    chord_tpl.cpp
    fft.cpp
    lmhelpers.cpp
    lmpriority_queue.cpp
    music_scale.cpp
    pitch_calculator.cpp
    pitch_cls_profile.cpp
    window_functions.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})