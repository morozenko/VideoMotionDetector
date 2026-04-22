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