#include <QGuiApplication>
#include <QQmlApplicationEngine>

// #include <gst/gst.h>
// #include <gst/gstplugin.h>

#include "GStreamerWorker/GStreamerWorker.h"

int main(int argc, char *argv[])
{
    // gst_init(nullptr, nullptr);

    // QString pluginPath = "C:/Program Files/gstreamer/1.0/mingw_x86_64/lib/gstreamer-1.0/libgstqml6.dll";
    // GstPlugin *plugin = gst_plugin_load_file(pluginPath.toUtf8().constData(), NULL);

    // gst_init() is called in GStreamerWorker constructor
    GStreamerWorker& gstWorker = GStreamerWorker::getInstance();
    gstWorker.CreateGstPipeline();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("VideoMotionDetector", "Main");

    return QCoreApplication::exec();
}
