#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <QLoggingCategory>

#include "GStreamerWorker/GStreamerWorker.h"
#include "ViewModels/VideoMotionDetectorViewModel.h"

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.qpa.gl=true\nqt.scenegraph.general=true");
    qputenv("QSG_RHI_BACKEND", "opengl");

    //Desktop OpenGL
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    // OpenGL profile (Core 3.3 or higher)
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    QSurfaceFormat::setDefaultFormat(format);

    QGuiApplication app(argc, argv);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    // gst_init() is called in GStreamerWorker constructor
    GStreamerWorker& gstWorker = GStreamerWorker::getInstance();
    gstWorker.CreateGstPipeline();

    // create ViewModel
    VideoMotionDetectorViewModel viewModel(gstWorker);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("VideoMotionDetector", "Main");

    QQuickItem* videoItem;
    QQuickWindow* rootObject;

    /* find and set the videoItem on the sink */
    rootObject = static_cast<QQuickWindow *> (engine.rootObjects().first());
    videoItem = rootObject->findChild<QQuickItem*> ("videoOutputItem");
    g_assert (videoItem);
    gstWorker.setVideoSink(videoItem);

    // Update gstreamer context, after QML is initialized
    GstElement* sink = gstWorker.getSink();
    auto connection = QObject::connect(rootObject, &QQuickWindow::beforeRendering, rootObject, [sink, videoItem]() {
        if (sink) {
            // Set window directly into sink
            // It force qml6glsink to take context, created by QT
            g_object_set(sink, "widget", videoItem, NULL);
            GStreamerWorker::getInstance().startPlaying();
        }
    }, Qt::SingleShotConnection);

    return QCoreApplication::exec();
}
