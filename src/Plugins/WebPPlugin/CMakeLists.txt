set(MY_LIB_NAME WebPPlugin)

ADD_FILTER(
src
	WebPPlugin.h
	WebPPlugin.cpp
    WebPPlugin.def
    
    ImageDecoderWebP.h
    ImageDecoderWebP.cpp
)
    
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/libwebp/src)

ADD_MENGINE_LIBRARY(Plugins)

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}webp${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}webpdecoder${MENGINE_LIB_SUFFIX})



