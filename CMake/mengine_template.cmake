set_property(GLOBAL PROPERTY USE_FOLDERS ON)

SET(MENGINE_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/../..)

IF(NOT MENGINE_INSTALL_PATH)
    SET(MENGINE_INSTALL_PATH ${MENGINE_REPOSITORY}/bin)
ENDIF()

MESSAGE("MENGINE_INSTALL_PATH: ${MENGINE_INSTALL_PATH}")

if(ANDROID)    
    # input and thirdparty paths
    SET(THIRDPARTY_DIR ${MENGINE_REPOSITORY}/dependencies)
    SET(THIRDPARTY_LIB_DIR ${MENGINE_REPOSITORY}/lib/dependencies/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})

    # output paths
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})
elseif(MINGW)
    # input and thirdparty paths
    SET(THIRDPARTY_DIR ${MENGINE_REPOSITORY}/dependencies)
    SET(THIRDPARTY_LIB_DIR ${MENGINE_REPOSITORY}/lib/dependencies/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})

    # output paths
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})
else()
    # input and thirdparty paths
    SET(THIRDPARTY_DIR ${MENGINE_REPOSITORY}/dependencies)
    SET(THIRDPARTY_LIB_DIR ${MENGINE_REPOSITORY}/lib/dependencies/${CMAKE_GENERATOR}/${CMAKE_BUILD_TYPE})
    
    # output paths
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MENGINE_REPOSITORY}/lib/${PROJECT_NAME}/${CMAKE_GENERATOR})
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MENGINE_INSTALL_PATH}/${PROJECT_NAME}/${CMAKE_GENERATOR})
endif()

MESSAGE("CMAKE_ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
MESSAGE("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

SET(MENGINE_CMAKE_DEPENDENCIES_DIR ${MENGINE_REPOSITORY}/CMake/Dependencies)
SET(MENGINE_SOURCE_DIR ${MENGINE_REPOSITORY}/src)
SET(CMAKE_TEMP_DIR ${CMAKE_BINARY_DIR}/solutions)

if(MENGINE_BUILD_MENGINE_MASTER_RELEASE)
    ADD_DEFINITIONS(-DMENGINE_MASTER_RELEASE)
endif()

if(MENGINE_USE_SCRIPT_SERVICE)
    ADD_DEFINITIONS(-DMENGINE_USE_SCRIPT_SERVICE)
endif()

INCLUDE_DIRECTORIES(${MENGINE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/libmath/include)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/stdex/include)

SET(APPLICATION_DEPENDENCIES)
SET(APPLICATION_LINK_LIBRARIES)
