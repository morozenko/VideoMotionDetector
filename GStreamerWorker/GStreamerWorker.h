#ifndef GSTREAMERWORKER_H
#define GSTREAMERWORKER_H

#include <gst/gst.h>

class QObject;

class GStreamerWorker
{
public:
    static GStreamerWorker& getInstance();
    void CreateGstPipeline();
    GstElement* getSink() const;
    void setVideoSink(QObject* sink);

    void startPlaying() const;

    ~GStreamerWorker();

private:
    GStreamerWorker();
    GStreamerWorker(const GStreamerWorker&) = delete;
    GStreamerWorker& operator=(const GStreamerWorker& instance) = delete;

private:
    GstElement* m_pipeline;
    GstElement* m_source;
    GstElement* m_capsfilter;
    GstElement* m_download;
    GstElement* m_convert;
    GstElement* m_upload;
    GstElement* m_sink;
};

#endif // GSTREAMERWORKER_H
