#include <QDebug>
#include <gst/gst.h>

#include "GStreamerWorker.h"

GStreamerWorker::GStreamerWorker()
{
    gst_init(nullptr, nullptr);
}

GStreamerWorker& GStreamerWorker::getInstance()
{
    static GStreamerWorker instance;
    return instance;
}

void GStreamerWorker::CreateGstPipeline()
{
    GstElement *sink = gst_element_factory_make("qml6glsink", NULL);

    if (sink == nullptr)
    {
        qDebug() << "ERROR: qml6glsink not found. Check GST_PLUGIN_PATH!";
    }
}