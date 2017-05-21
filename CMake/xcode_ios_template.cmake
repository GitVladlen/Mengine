# SET LIBNAME
SET( MENGINE_LIB_PREFIX "lib" )
SET( MENGINE_LIB_SUFFIX ".a" )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch armv7 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch armv7 -std=c++11 -stdlib=libc++ -std=c++1y ")

set(LLVM_ENABLE_CXX1Y ON CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")


set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")