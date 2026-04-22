cmake_minimum_required(VERSION 3.16)

project(VideoMotionDetector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GSTREAMER_ROOT "C:/Program Files/gstreamer/1.0/mingw_x86_64")

find_package(Qt6 REQUIRED COMPONENTS Quick)

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
)

target_include_directories(appVideoMotionDetector PRIVATE "${GSTREAMER_ROOT}/include/gstreamer-1.0")
target_include_directories(appVideoMotionDetector PRIVATE "${GSTREAMER_ROOT}/include/glib-2.0")
target_include_directories(appVideoMotionDetector PRIVATE "${GSTREAMER_ROOT}/lib/glib-2.0/include")
target_link_directories(appVideoMotionDetector PRIVATE "${GSTREAMER_ROOT}/lib")

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
)

install(TARGETS appVideoMotionDetector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
