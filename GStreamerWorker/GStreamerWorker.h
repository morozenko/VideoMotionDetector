#ifndef GSTREAMERWORKER_H
#define GSTREAMERWORKER_H

#include <gst/gst.h>
#include <stdint.h>

class QObject;

class GStreamerWorker
{
public:
    static GStreamerWorker& getInstance();
    void CreateGstPipeline();
    GstElement* getSink() const;
    void setVideoSink(QObject* sink);

    void startPlaying() const;
    void updateVideoFrameSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    ~GStreamerWorker();

private:
    GStreamerWorker();
    GStreamerWorker(const GStreamerWorker&) = delete;
    GStreamerWorker& operator=(const GStreamerWorker& instance) = delete;

    void createPipelineElements();
    void setPipelineProperties(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

private:
    GstElement* m_pipeline;
    GstElement* m_source;
    GstElement* m_capsfilter;
    GstElement* m_download;
    GstElement* m_tee;          // video splitter
    GstElement* m_origQueue;
    GstElement* m_delay;
    GstElement* m_invert;
    GstElement* m_mixer;        // video compositor
    GstElement* m_convert;
    GstElement* m_upload;
    GstElement* m_sink;
};

#endif // GSTREAMERWORKER_H
