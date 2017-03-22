ADD_DEFINITIONS (-DHAVE_EXPAT_CONFIG_H)

INCLUDE_DIRECTORIES (${EXPAT_DIR}/src ${EXPAT_DIR}/include)
#LINK_DIRECTORIES (${EXPAT_DIR}/lib)

IF (NOT UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows -mwindows")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows -mwindows")
ENDIF(NOT UNIX)

# Sources to compile and integrate in executable

SET (SOURCES xmlparse.c xmltok.c xmlrole.c)

# Main executable

ADD_LIBRARY (expat STATIC ${SOURCES})