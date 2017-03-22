# Sources to compile and integrate in executable

SET(LIB_NAME "tslib")

SET (SOURCES src/bit.cpp
             src/ts/table.cpp
             src/ts/ts.cpp
             src/ts/packet.cpp
             src/config.cpp
             src/logger.cpp)

# wxWidgets include headers

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

INCLUDE_DIRECTORIES(${WXWIDGETS_INCLUDE_DIRS})

# Main executable

ADD_LIBRARY(${LIB_NAME} ${SOURCES})

#SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES LINK_FLAGS_RELEASE "")
#SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES LINK_FLAGS_DEBUG "")
#SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "")

# Link system libraries

IF (UNIX)
  TARGET_LINK_LIBRARIES(${LIB_NAME} m rt)
ELSE (UNIX)
ENDIF (UNIX)

# Link GCC compiler statically

IF (NOT UNIX)
  #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows -mwindows")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
ENDIF(NOT UNIX)