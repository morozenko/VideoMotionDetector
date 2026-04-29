#include <QDebug>

#include "GStreamerWorker.h"

GStreamerWorker::GStreamerWorker()
{
    gst_init(nullptr, nullptr);
}

GStreamerWorker::~GStreamerWorker()
{
    // gst_element_set_state(m_sink, GST_STATE_NULL);
    // gst_element_set_state(m_upload, GST_STATE_NULL);
    // gst_element_set_state(m_convert, GST_STATE_NULL);
    // gst_element_set_state(m_download, GST_STATE_NULL);
    // gst_element_set_state(m_capsfiter, GST_STATE_NULL);
    // gst_element_set_state(m_source, GST_STATE_NULL);

    // gst_bin_remove(GST_BIN(m_pipeline), m_sink);
    // gst_bin_remove(GST_BIN(m_pipeline), m_upload);
    // gst_bin_remove(GST_BIN(m_pipeline), m_convert);
    // gst_bin_remove(GST_BIN(m_pipeline), m_download);
    // gst_bin_remove(GST_BIN(m_pipeline), m_capsfilter);
    // gst_bin_remove(GST_BIN(m_pipeline), m_source);

    // gst_object_unref(m_sink);
    // gst_object_unref(m_upload);
    // gst_object_unref(m_convert);
    // gst_object_unref(m_download);
    // gst_object_unref(m_source);
    gst_object_unref(m_pipeline); // should unref all elemnts in pipeline too
}

GStreamerWorker& GStreamerWorker::getInstance()
{
    static GStreamerWorker instance;
    return instance;
}

GstElement* GStreamerWorker::getSink() const
{
    return m_sink;
}

void GStreamerWorker::setVideoSink(QObject* sink)
{
    if (m_sink && sink)
    {
        g_object_set(m_sink, "widget", sink, nullptr);

        // start pipeline
        auto ret = gst_element_set_state(m_pipeline, GST_STATE_PLAYING);

        if (GST_STATE_CHANGE_FAILURE == ret)
        {
            qDebug() << "ERROR: cant change state to PLAYING!";
            return;
        }
    }
}

void GStreamerWorker::CreateGstPipeline()
{
    m_pipeline = gst_pipeline_new("screen-capture-pipeline");
    m_source = gst_element_factory_make("d3d11screencapturesrc", "source");
    m_capsfilter = gst_element_factory_make("capsfilter", "filter");
    m_download = gst_element_factory_make("d3d11download", "download");
    m_convert = gst_element_factory_make("videoconvert", "convert");
    m_upload = gst_element_factory_make("glupload", "upload");
    m_sink = gst_element_factory_make("qml6glsink", "mysink");
    //GstElement* sink = gst_element_factory_make("qml6glsink", "mysink");

    if ((nullptr == m_pipeline) || (nullptr == m_source) ||
        (nullptr == m_capsfilter) || (nullptr == m_download) ||
        (nullptr == m_convert) || (nullptr == m_upload) ||
        (nullptr == m_sink))
    {
        qDebug() << "ERROR: couldn't initiate pipeline!";
    }

    GstCaps* caps = gst_caps_from_string("video/x-raw,format=I420");
    g_object_set(G_OBJECT(m_capsfilter), "caps", caps, NULL);
    gst_caps_unref(caps);

    gst_bin_add_many(GST_BIN(m_pipeline),
                     m_source,
                     //m_capsfilter,
                     m_download,
                     m_convert,
                     m_upload,
                     m_sink,
                     nullptr);

    if (!gst_element_link_many(m_source, /*m_capsfilter,*/ m_download, m_convert, m_upload, m_sink, nullptr))
    {
        qDebug() << "ERROR: couldn't link pipeline elements!";
        return;
    }
}

void GStreamerWorker::startPlaying() const
{
    // start pipeline
    auto ret = gst_element_set_state(m_pipeline, GST_STATE_PLAYING);

    if (GST_STATE_CHANGE_FAILURE == ret)
    {
        qDebug() << "ERROR: cant change state to PLAYING!";
        return;
    }
}