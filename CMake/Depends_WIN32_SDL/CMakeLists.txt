CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

OPTION(MENGINE_USE_PRECOMPILED_HEADER "Mengine use precompiled header" OFF)

PROJECT(Depends_WIN32_SDL)

INCLUDE(../macro_template.cmake)
INCLUDE(../win32_template.cmake)

SET_MENGINE_ENVIRONMENT(WINDOWS OPENGL X86 SDL MSVC)

SET(MENGINE_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/../..)
SET(THIRDPARTY_DIR ${MENGINE_REPOSITORY}/dependencies)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/dependencies/${CMAKE_GENERATOR})

SET(CMAKE_TEMP_DIR ${CMAKE_BINARY_DIR}/Dependencies)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies ${CMAKE_TEMP_DIR})

