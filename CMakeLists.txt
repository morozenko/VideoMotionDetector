cmake_minimum_required(VERSION 3.16)

project(VideoMotionDetector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOGEN_SKIP_PATHS "C:/Program Files/gstreamer")
set(GSTREAMER_ROOT "C:/Program Files/gstreamer/1.0/mingw_x86_64")
set(GST_PLUGIN_PATH "C:/Program Files/gstreamer/1.0/mingw_x86_64/lib/gstreamer-1.0")

find_package(Qt6 REQUIRED COMPONENTS Quick)

# set(PKG_CONFIG_EXECUTABLE "C:/Qt/Tools/mingw1310_64/bin/pkg-config.exe")
# find_package(PkgConfig REQUIRED)

# pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
# pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appVideoMotionDetector
    main.cpp
)

qt_add_qml_module(appVideoMotionDetector
    URI VideoMotionDetector
    QML_FILES
        Main.qml
        SOURCES GStreamerWorker/GStreamerWorker.cpp
        SOURCES GStreamerWorker/GStreamerWorker.h
        SOURCES ViewModels/VideoMotionDetectorViewModel.h
        SOURCES ViewModels/VideoMotionDetectorViewModel.cpp
)

target_precompile_headers(appVideoMotionDetector PRIVATE
    <QObject>
    <QQuickItem>
    <gst/gst.h>
    <gst/video/video.h>
)

target_include_directories(appVideoMotionDetector SYSTEM PRIVATE "${GSTREAMER_ROOT}/include/gstreamer-1.0")
target_include_directories(appVideoMotionDetector SYSTEM PRIVATE "${GSTREAMER_ROOT}/include/glib-2.0")
target_include_directories(appVideoMotionDetector SYSTEM PRIVATE "${GSTREAMER_ROOT}/lib/glib-2.0/include")
target_link_directories(appVideoMotionDetector PRIVATE "${GSTREAMER_ROOT}/lib")
target_link_directories(appVideoMotionDetector PRIVATE "${GSTREAMER_ROOT}/lib/streamer-1.0")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appVideoMotionDetector PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appVideoMotionDetector
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appVideoMotionDetector
    PRIVATE Qt6::Quick
    PRIVATE gstreamer-1.0
    PRIVATE gstvideo-1.0
    glib-2.0
    gobject-2.0
)

install(TARGETS appVideoMotionDetector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
