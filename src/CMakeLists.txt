#
# Transport Stream Analyzer
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(APPLICATION)

SET (PROJECT_NAME "transport-stream-analyzer")

SET (EXECUTABLE_OUTPUT_PATH ${APPLICATION_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${APPLICATION_BINARY_DIR}/lib)

MESSAGE("PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
MESSAGE("APPLICATION_SOURCE_DIR=${APPLICATION_SOURCE_DIR}")

SET (WXWIDGETS_VERSION_MAJOR 3.1)

IF (UNIX)
  SET (OPERATING_SYSTEM "unix")
ELSE (UNIX)
  SET (OPERATING_SYSTEM "windows")
ENDIF (UNIX)

# Linux

IF (UNIX)
  # Subfolder inside folder third/wxWidgets/dist/linux/ (eg. fc20, debian73)
  SET (OS_DISTRO debian87)
ELSE (UNIX)
  # win32, win64
  SET (OS_DISTRO win32)
  SET (OS_BUILDTYPE "msys")
ENDIF (UNIX)

SET (CMAKE_VERBOSE_MAKEFILE "ON")

# Compiler flags for release mode
SET (CMAKE_C_FLAGS_RELEASE "")
SET (CMAKE_CXX_FLAGS_RELEASE "")
SET (CMAKE_C_FLAGS_RELEASE_INIT "")
SET (CMAKE_CXX_FLAGS_RELEASE_INIT "")

# Compiler flags for debug mode
SET (CMAKE_C_FLAGS_DEBUG "")
SET (CMAKE_CXX_FLAGS_DEBUG "")
SET (CMAKE_C_FLAGS_DEBUG_INIT "")
SET (CMAKE_CXX_FLAGS_DEBUG_INIT "")

# Release | Debug
SET (CMAKE_BUILD_TYPE Release)

SET (LIBSNDFILE_ACTIVATED FALSE)
SET (SNDLIB_ACTIVATED FALSE)
SET (ICONV_ACTIVATED FALSE)

# Expat configuration

SET (EXPAT_DIR ${APPLICATION_SOURCE_DIR}/app/third/expat)

# wxWidgets configuration

SET(WXWIDGETS_DIR ${APPLICATION_SOURCE_DIR}/third/wxWidgets)

# Build definitions

IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
ELSE ()
  ADD_COMPILE_OPTIONS (-DwxDEBUG_LEVEL=0)

  IF (NOT MSVC)
    ADD_DEFINITIONS(-DNDEBUG)
  ELSE ()
    ADD_DEFINITIONS(-NDEBUG)
  ENDIF ()
ENDIF ()

ADD_COMPILE_OPTIONS (-g)
ADD_COMPILE_OPTIONS (-Wall)
#ADD_COMPILE_OPTIONS (-std=c++11)
ADD_COMPILE_OPTIONS (-std=gnu++11)

# Trigraphs

ADD_COMPILE_OPTIONS (-trigraphs)

# wxWidgets

SET (WXWIDGETS_INCLUDE_DIRS "")

IF (${CMAKE_BUILD_TYPE} MATCHES Debug)
  IF (NOT MSVC)
    ADD_COMPILE_OPTIONS (-g -Wall)
  ENDIF ()
ELSE ()
  ADD_COMPILE_OPTIONS (-Wall)
ENDIF ()

ADD_COMPILE_OPTIONS (-Wall)

# wxWidgets - Linux / Unix

IF (UNIX)
  SET (WXWIDGETS_VERSION_SUFFIX ${WXWIDGETS_VERSION_MAJOR})
  SET (WXWIDGETS_UNICODE_SUFFIX "u")
  SET (WXWIDGETS_DEBUG_SUFFIX "")
  # SET (WXWIDGETS_RELEASE_PART_NAME "")
  SET (WXWIDGETS_RELEASE_PART_NAME "")

  SET (WXWIDGETS_PLATFORM_SUFFIX "gtk3")

  #ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -g -pthread -Wall -DwxDEBUG_LEVEL=0)
  ADD_DEFINITIONS (-D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__)

  ADD_COMPILE_OPTIONS(-g -pthread)

  SET (WXWIDGETS_LIBRARIES
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_richtext-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_adv-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_html-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_aui-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_propgrid-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_core-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_qa-${WXWIDGETS_VERSION_SUFFIX}
                           #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_gl-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_xrc-${WXWIDGETS_VERSION_SUFFIX}
                           #wx_${WXWIDGETS_PLATFORM_SUFFIX}u_media-${WXWIDGETS_VERSION_SUFFIX}
                           wx_${WXWIDGETS_PLATFORM_SUFFIX}u_ribbon-${WXWIDGETS_VERSION_SUFFIX}
                           wx_baseu_xml-${WXWIDGETS_VERSION_SUFFIX}
                           wx_baseu_net-${WXWIDGETS_VERSION_SUFFIX}
                           wx_baseu-${WXWIDGETS_VERSION_SUFFIX}
                           wxregexu-${WXWIDGETS_VERSION_SUFFIX}

                           expat
                           gtk-3
                           gdk-3
                           pangocairo-1.0
                           pango-1.0
                           atk-1.0
                           cairo-gobject
                           cairo
                           gdk_pixbuf-2.0
                           gio-2.0
                           gobject-2.0
                           gthread-2.0
                           glib-2.0
                           X11
                           Xxf86vm
                           SM
                           notify
                           webkitgtk-3.0
                           soup-2.4
                           javascriptcoregtk-3.0
                           png
                           z
                           jpeg
                           tiff
                           pthread
                           dl
                           m)

    SET(WXWIDGETS_LIBRARIES_DIR ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/lib)

    #INCLUDE_DIRECTORIES (${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-release-static-${WXWIDGETS_VERSION_SUFFIX}
    #                     ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_MAJOR})

  SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_INCLUDE_DIRS}
  # gtk2-unicode-3.0
  # ./lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-3.0/wx/setup.h
                              ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode${WXWIDGETS_RELEASE_PART_NAME}-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode-static-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/lib/wx/include/${WXWIDGETS_PLATFORM_SUFFIX}-unicode${WXWIDGETS_RELEASE_PART_NAME}-${WXWIDGETS_VERSION_SUFFIX}
                              ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_MAJOR})

  SET(WXWIDGETS_LIBRARIES_DIRS /usr/X11R6/lib
                               ${WXWIDGETS_DIR}/dist/linux/${OS_DISTRO}/lib)
# wxWidgets - Windows

ELSE (UNIX)
  SET (WXWIDGETS_VERSION_INCLUDE_DIR "3.1")
  SET (WXWIDGETS_VERSION_SUFFIX "31")
  SET (WXWIDGETS_UNICODE_SUFFIX "u")
  SET (WXWIDGETS_DEBUG_SUFFIX "")

  #ADD_DEFINITIONS(-DwxDEBUG_LEVEL=0 -D__WXMSW__ -mwindows -mthreads)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNICODE -D_UNICODE -mthreads -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual")

  IF (OS_BUILDTYPE STREQUAL "msys")
    SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib/wx/include/msw-unicode-static-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib/mswu
                                ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib/msw${WXWIDGETS_UNICODE_SUFFIX})

    SET(WXWIDGETS_LIBRARIES wx_mswu_aui-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wx_baseu_xml-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wx_mswu_html-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wx_mswu_adv-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wx_mswu_core-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wx_baseu-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wxtiff-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wxjpeg-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wxpng-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wxzlib-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wxregexu-${WXWIDGETS_VERSION_INCLUDE_DIR}
                            wxexpat-${WXWIDGETS_VERSION_INCLUDE_DIR}

                            rpcrt4
                            oleaut32
                            ole32
                            uuid
                            winspool
                            winmm
                            shell32
                            shlwapi
                            comctl32
                            comdlg32
                            advapi32
                            version
                            wsock32
                            gdi32
                            oleacc

                            #kernel32
                            #user32
                            #gdi32
                            #comdlg32
                            #winspool
                            #winmm
                            #shell32
                            #comctl32
                            #ole32
                            #oleaut32
                            #uuid
                            #rpcrt4
                            #advapi32
                            #wsock32
                            #wininet
                            )
    SET(WXWIDGETS_LIBRARIES_DIR ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib)

  ELSE ()

    SET (WXWIDGETS_INCLUDE_DIRS ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib
                                ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/include/wx-${WXWIDGETS_VERSION_INCLUDE_DIR}
                                ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib/mswu)

    SET(WXWIDGETS_LIBRARIES kernel32
                            user32
                            gdi32
                            comdlg32
                            winspool
                            winmm
                            shell32
                            comctl32
                            ole32
                            oleaut32
                            uuid
                            rpcrt4
                            advapi32
                            wsock32
                            wininet)
  ENDIF ()

  SET(WXWIDGETS_LIBRARIES_DIR ${WXWIDGETS_DIR}/dist/${OS_DISTRO}/lib)

  #INCLUDE_DIRECTORIES (${WXWIDGETS_DIR}/include
  #                     ${WXWIDGETS_DIR}/lib/vc_lib/msw${WXWIDGETS_UNICODE_SUFFIX}${WXWIDGETS_DEBUG_SUFFIX})
ENDIF (UNIX)

MESSAGE("WXWIDGETS_LIBRARIES_DIR=${WXWIDGETS_LIBRARIES_DIR}")

# Headers and Transport Stream library

SET (TS_LIB_INCLUDE_DIRS ${APPLICATION_SOURCE_DIR}/lib/include)

INCLUDE_DIRECTORIES(${APPLICATION_SOURCE_DIR}/lib/include)

# Process subdirectories

ADD_SUBDIRECTORY(third)

ADD_SUBDIRECTORY(lib)

ADD_SUBDIRECTORY(app)