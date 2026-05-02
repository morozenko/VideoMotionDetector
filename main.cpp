#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlContext>

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
    // WDA_EXCLUDEFROMCAPTURE = 0x00000011
    SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);
}

int main(int argc, char *argv[])
{
    qputenv("GST_DEBUG", "3");

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
