SET(MY_LIB_NAME SDLSocketSystem)

ADD_FILTER(
src
    SDLSocket.h
    SDLSocket.cpp
    SDLSocketSystem.h
    SDLSocketSystem.cpp
)

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/SDL2/include)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/SDL2_net)

ADD_MENGINE_LIBRARY(Systems)

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}SDL2_net${MENGINE_LIB_SUFFIX})