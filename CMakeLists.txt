cmake_minimum_required(VERSION 3.16)

project(VideoMotionDetector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOGEN_SKIP_PATHS "C:/Program Files/gstreamer")
set(GSTREAMER_ROOT "C:/Program Files/gstreamer/1.0/mingw_x86_64")
set(GST_PLUGIN_PATH "C:/Program Files/gstreamer/1.0/mingw_x86_64/lib/gstreamer-1.0")

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

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
        SOURCES ViewModels/FilterWindowViewModel.h
        SOURCES ViewModels/FilterWindowViewModel.cpp
        SOURCES Logger/ApplicationLogger.h
        SOURCES Logger/ApplicationLogger.cpp
        SOURCES VideoManager/VideoDeviceManager.h
        SOURCES VideoManager/VideoDeviceManager.cpp
        QML_FILES qml/FilterScreenWindow.qml
        SOURCES ViewModels/StartWindowViewModel.h
        SOURCES ViewModels/StartWindowViewModel.cpp
        SOURCES ApplicationController.h
        SOURCES ApplicationController.cpp
        QML_FILES qml/FilterScreenWindow.qml
        SOURCES ViewModels/FilterWindowViewModel.cpp ViewModels/FilterWindowViewModel.h
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
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appVideoMotionDetector
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Multimedia
    PRIVATE gstreamer-1.0
    PRIVATE gstvideo-1.0
    glib-2.0
    gobject-2.0
)

# install(TARGETS appVideoMotionDetector
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

if(WIN32)
    set(DEPLOY_PATH "D:/Projects/Deploy")
    set(CMAKE_INSTALL_PREFIX "D:/Projects/Deploy")

    # create deploy script for Qt 6
    qt_generate_deploy_app_script(
        TARGET appVideoMotionDetector
        OUTPUT_SCRIPT deploy_script
    )

    # qt_generate_deploy_app_script(
    #     TARGET appVideoMotionDetector
    #     OUTPUT_SCRIPT deploy_script
    #     # Add this, than script will know were to look for imports
    #     DEPLOY_USER_OPTIONS "--qmldir;${CMAKE_CURRENT_SOURCE_DIR}"
    # )

    # File copy rules
    install(TARGETS appVideoMotionDetector
        RUNTIME DESTINATION ${DEPLOY_PATH}
    )

    # install(TARGETS appVideoMotionDetector
    #     RUNTIME DESTINATION .
    # )

    # run deploy script
    install(SCRIPT ${deploy_script})

    # additionally copy script to target folder
    set(QT_DEPLOY_PREFIX ${DEPLOY_PATH})
endif()

