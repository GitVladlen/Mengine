set(MY_LIB_NAME Movie1Plugin)

ADD_FILTER(
src
    Movie1Interface.h
    
	Movie1Plugin.h
	Movie1Plugin.cpp
    
    Movie.h
    Movie.cpp
    MovieSlot.h
    MovieSlot.cpp    
    MovieSceneEffect.h
    MovieSceneEffect.cpp
    MovieNodeExtra.h
    MovieNodeExtra.cpp
    MovieInternalObject.h
    MovieInternalObject.cpp
    MovieEvent.h
    MovieEvent.cpp
    MovieMesh2D.h
    MovieMesh2D.cpp
    ResourceMovie.cpp
    ResourceMovie.h
    ResourceInternalObject.cpp
    ResourceInternalObject.h
    LoaderResourceMovie.cpp
    LoaderResourceMovie.h
    LoaderResourceInternalObject.cpp
    LoaderResourceInternalObject.h
    
    ResourceMovieValidator.cpp
    ResourceMovieValidator.h
    
    DataflowAEK.h
    DataflowAEK.cpp
    MovieFramePack.h
    MovieFramePack.cpp
)

if(MENGINE_USE_SCRIPT_SERVICE)
    ADD_FILTER(
    embedding
        MovieScriptEmbedding.cpp
        MovieScriptEmbedding.h
        )    

    INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/pybind/include)
endif()

add_definitions(-DMETABUF_EXTERNAL_CONFIG)

INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/pybind/include)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/metabuf/include)

ADD_MENGINE_PLUGIN(MENGINE_PLUGIN_MOVIE1)

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}stdex${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}math${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}pybind${MENGINE_LIB_SUFFIX})
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} ${THIRDPARTY_LIB_DIR}/${MENGINE_LIB_PREFIX}metabuf${MENGINE_LIB_SUFFIX})

TARGET_LINK_LIBRARIES(${MY_LIB_NAME} Kernel)
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} Engine)
TARGET_LINK_LIBRARIES(${MY_LIB_NAME} Metacode)