#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickStyle>
#include <QQmlContext>
#include <QMessageBox>

#include <memory>
#include <windows.h>

#include "GStreamerWorker/GStreamerWorker.h"
#include "ViewModels/VideoMotionDetectorViewModel.h"

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
    try
    {
        // uncomment to have detailed gstreamer logs
        // qputenv("GST_DEBUG", "3");

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
    catch (const std::exception& e)
    {
        QMessageBox::critical(nullptr, "ERROR", e.what());
    }
}
