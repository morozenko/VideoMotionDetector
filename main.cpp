/**
 * @file    main.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   Here app is created, initialized
 *          Main window has property, to avoid self caturing
 *
 * @version 1.0
 * @date    2026-05-05
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickStyle>
#include <QQmlContext>

#include <memory>
#include <windows.h>
#include <io.h>

#include "GStreamerWorker/GStreamerWorker.h"
#include "ViewModels/VideoMotionDetectorViewModel.h"
#include "Logger/ApplicationLogger.h"

void redirectAllOutputToFile(const char* fileName)
{
    // Create/Open the file
    QFile* file = new QFile(fileName);
    if (file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {

        // get windows handle
        HANDLE hFile = (HANDLE)_get_osfhandle(file->handle());

        // redirect system output (to capture linked DLL logs)
        SetStdHandle(STD_OUTPUT_HANDLE, hFile);
        SetStdHandle(STD_ERROR_HANDLE, hFile);

        // redirect C streams (printf/fprintf)
        _dup2(file->handle(), _fileno(stdout));
        _dup2(file->handle(), _fileno(stderr));

        // switch off buffering
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
    }
}

void setNonCapturable(QQuickWindow *window)
{
    if (!window)
    {
        return;
    }

    HWND hwnd = (HWND)window->winId();

    // Set WS_EX_APPWINDOW flag, to display window on taskbar
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_APPWINDOW;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

    // WDA_EXCLUDEFROMCAPTURE = 0x00000011
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
}

int main(int argc, char *argv[])
{
    // uncomment to have detailed gstreamer logs
    qputenv("GST_DEBUG", "3");

    ApplicationLogger& logger = ApplicationLogger::getInstance();
    qInstallMessageHandler(logger.messageHandler);
    redirectAllOutputToFile(ApplicationLogger::getLogFileName());

    QQuickStyle::setStyle("Material");
    QGuiApplication app(argc, argv);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    // gst_init() is called in GStreamerWorker constructor
    GStreamerWorker& gstWorker = GStreamerWorker::getInstance();
    gstWorker.CreateGstPipeline();

    // create ViewModel
    std::shared_ptr<VideoMotionDetectorViewModel> viewModel =
        std::make_shared<VideoMotionDetectorViewModel>(gstWorker);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("appViewModel", viewModel.get());

    engine.loadFromModule("VideoMotionDetector", "Main");

    QQuickItem* videoItem;
    QQuickWindow* rootObject;

    // find and set the videoItem on the sink
    rootObject = static_cast<QQuickWindow*> (engine.rootObjects().first());
    videoItem = rootObject->findChild<QQuickItem*> ("videoOutputItem");
    g_assert (videoItem);

    // set window not capturable
    setNonCapturable(rootObject);

    // Update gstreamer context, after QML is initialized
    GstElement* sink = gstWorker.getSink();
    auto connection = QObject::connect(rootObject, &QQuickWindow::beforeRendering, rootObject, [sink, videoItem, rootObject]() {
        // update captured frame size
        GStreamerWorker::getInstance().updateVideoFrameSize(rootObject->x(),
                                                            rootObject->y(),
                                                            rootObject->width(),
                                                            rootObject->height());

        if (sink) {
            // Set window directly into sink
            // It force qml6glsink to take context, created by QT
            g_object_set(sink, "widget", videoItem, nullptr);
            GStreamerWorker::getInstance().startPlaying();
        }
    }, Qt::SingleShotConnection);

    return QCoreApplication::exec();
}
