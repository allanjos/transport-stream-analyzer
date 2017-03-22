# Executable name

SET(APP_NAME "tsgui")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LARGEFILE_SOURCE=unknown")

IF (NOT UNIX)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WXMSW__ -mwindows ")
ENDIF()

# Sources

SET (SOURCES src/general/dlg_settings.cpp
             src/general/dlg_about.cpp
             src/general/dlg_main.cpp
             src/app/app.cpp)

# wxWidgets headers/link directories

INCLUDE_DIRECTORIES(${WXWIDGETS_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

#LINK_DIRECTORIES(${WXWIDGETS_LIBRARIES_DIRS})

# Expat headers/link directories

INCLUDE_DIRECTORIES(${EXPAT_DIR}/include)

LINK_DIRECTORIES(${WXWIDGETS_LIBRARIES_DIR})

# Main executable

IF (UNIX)
  ADD_EXECUTABLE(${APP_NAME} ${SOURCES})
ELSE (UNIX)
  #LINK_DIRECTORIES(${APPLICATION_SOURCE_DIR}/third/wxWidgets/dist/win32/lib)

  # Resources

  ADD_CUSTOM_COMMAND(OUTPUT app_rc.o
                     COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
                                         -I${WXWIDGETS_DIR}/dist/win32/lib
                                         -I${WXWIDGETS_DIR}/dist/win32/include
                                         -I${WXWIDGETS_DIR}/dist/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                         -I${WXWIDGETS_DIR}/dist/win32/lib/mswu
                                         -I${WXWIDGETS_DIR}/dist/win32/lib/wx/include/msw-unicode${WXWIDGETS_RELEASE_PART_NAME}-static-${WXWIDGETS_VERSION_SUFFIX}
                                         -i${CMAKE_CURRENT_SOURCE_DIR}/src/app/app.rc
                                         -o ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)

  ADD_EXECUTABLE(${APP_NAME} WIN32 ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/app_rc.o)
ENDIF (UNIX)

# wxWidgets library linking

TARGET_LINK_LIBRARIES(${APP_NAME} ${WXWIDGETS_LIBRARIES})

# Link Transport Stream library

TARGET_LINK_LIBRARIES(${APP_NAME} tslib)

# Link system libraries

IF (UNIX)
  TARGET_LINK_LIBRARIES(${APP_NAME} m rt)
ENDIF (UNIX)

# Link GCC compiler statically

IF (NOT UNIX)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -mthreads -Wl,--subsystem,windows")
ENDIF (NOT UNIX)