# Sources to compile and integrate in executable

SET(APP_NAME "tsparser")

SET (SOURCES src/application.cpp
	         src/main.cpp)

# wxWidgets include headers

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

INCLUDE_DIRECTORIES(${APPLICATION_SOURCE_DIR}/lib/include)

# Resources

#ADD_CUSTOM_COMMAND(OUTPUT app_rc.o
#                   COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
#                                       -I${WXWIDGET_DIR}/build/win32/lib
#                                       -I${WXWIDGET_DIR}/build/win32/include
#                                       -I${WXWIDGET_DIR}/build/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_SUFFIX}
#                                       -I${WXWIDGET_DIR}/build/win32/lib/mswu
#                                       -I${WXWIDGET_DIR}/build/win32/lib/wx/include/msw-unicode${WXWIDGETS_RELEASE_PART_NAME}-static-${WXWIDGETS_VERSION_SUFFIX}
#                                       -i${CMAKE_CURRENT_SOURCE_DIR}/app.rc
#                                       -o ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)

# Main executable

IF (UNIX)
  ADD_EXECUTABLE(${APP_NAME} ${SOURCES})
ELSE (UNIX)
  #ADD_EXECUTABLE(${APP_NAME} WIN32 app.rc ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)
  ADD_EXECUTABLE (${APP_NAME} ${SOURCES})
ENDIF (UNIX)

#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES LINK_FLAGS_RELEASE "")
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES LINK_FLAGS_DEBUG "")
#SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "")

# wxWidgets library linking

#TARGET_LINK_LIBRARIES (${APP_NAME} ${WXWIDGETS_LIBRARIES})

# Link system libraries

IF (UNIX)
  TARGET_LINK_LIBRARIES(${APP_NAME} m rt)
ELSE (UNIX)
ENDIF (UNIX)

TARGET_LINK_LIBRARIES(${APP_NAME} tslib)

# Link GCC compiler statically

IF (NOT UNIX)
  #SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows -mwindows")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
ENDIF(NOT UNIX)