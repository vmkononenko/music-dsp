include(ExternalProject)

set (VAMP_BUILD_DIR ${CMAKE_BINARY_DIR}/${EXT_VAMPSDK_TARGET})

ExternalProject_Add(
    ${EXT_VAMPSDK_TARGET}
    DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${VAMP_BUILD_DIR}
    SOURCE_DIR ${VAMP_BUILD_DIR}
    CONFIGURE_COMMAND SNDFILE_LIBS=${LIBSNDFILE} SNDFILE_CFLAGS=-I${SND_HEADERS} ${VAMP_BUILD_DIR}/configure --prefix=${CMAKE_BINARY_DIR}
    PREFIX ${CMAKE_BINARY_DIR}
    BUILD_COMMAND make sdk
    BUILD_IN_SOURCE 1
)

add_library(vamp-sdk SHARED IMPORTED)
set_property(TARGET vamp-sdk PROPERTY IMPORTED_LOCATION ${VAMP_BUILD_DIR})
