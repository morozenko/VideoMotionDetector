/**
 * @file    GStreamerWorker.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   GStreamerWorker do all work with gstreamer - initialization, configuring,
 *          builds pipeline, starts, stop, etc.
 *          Screen is captured then two flows are blended.
 *          First flow is usual with 0.5 alpha transparency
 *          Second flow is color inverted, has 0.5 alpha transparency
 *          and delayed.
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

#include <QDebug>

#include "GStreamerWorker.h"

const uint32_t ONE_MILLION = 1000000;

const gchar *shader_source =
    "varying vec2 v_texcoord;"
    "uniform sampler2D tex;"
    "void main() {"
    "  vec4 rgba = texture2D(tex, v_texcoord);"
    "  gl_FragColor = vec4(1.0 - rgba.rgb, rgba.a);"
    "}";

GStreamerWorker::GStreamerWorker()
{
    m_msecDelay = 50;
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

uint16_t GStreamerWorker::getDelayValue()
{
    return m_msecDelay;
}

void GStreamerWorker::setDelayValue(uint16_t msecDelay)
{
    if (!gst_element_set_state(m_pipeline, GST_STATE_PAUSED))
    {
        qDebug() << "ERROR: Could not pause pipeline!";
    }

    GstPad *sink1 = gst_element_get_static_pad(m_mixer, "sink_1");

    if (sink1)
    {
        qDebug() << "Setting mixer delay";
        gst_pad_set_offset(sink1, (gint64)msecDelay*ONE_MILLION);
        gst_object_unref(sink1);

        m_msecDelay = msecDelay;
    }
    else
    {
        qDebug() << "Pad not found! Try request_pad approach.";
    }

    // flush old frames
    gst_element_send_event(m_pipeline, gst_event_new_flush_start());
    gst_element_send_event(m_pipeline, gst_event_new_flush_stop(TRUE));

    if (!gst_element_set_state(m_pipeline, GST_STATE_PLAYING))
    {
        qDebug() << "ERROR: Could not start pipeline!";
    }
}

void GStreamerWorker::createPipelineElements()
{
    m_pipeline = gst_pipeline_new("screen-capture-pipeline");
    m_source = gst_element_factory_make("d3d11screencapturesrc", "source"); //d3d11screencapturesrc
    m_capsfilter = gst_element_factory_make("capsfilter", "filter");
    m_download = gst_element_factory_make("d3d11download", "download");
    m_tee = gst_element_factory_make("tee", "tee");
    m_origQueue = gst_element_factory_make("queue", "origQueue");
    m_delay = gst_element_factory_make("queue", "delay");

    m_uploadShader = gst_element_factory_make("glupload", "glup_shader");
    m_shader = gst_element_factory_make("glshader", "invert");
    g_object_set(G_OBJECT(m_shader), "fragment", shader_source, nullptr);
    m_downloadShader = gst_element_factory_make("gldownload", "gldown_shader");

    m_mixer = gst_element_factory_make("compositor", "mixer");
    m_convert = gst_element_factory_make("videoconvert", "convert"); // glcolorconvert
    m_upload = gst_element_factory_make("glupload", "upload");
    m_sink = gst_element_factory_make("qml6glsink", "mysink");

    if ((nullptr == m_pipeline) || (nullptr == m_source) || (nullptr == m_capsfilter) ||
        (nullptr == m_download) || (nullptr == m_tee) || (nullptr == m_origQueue) ||
        (nullptr == m_delay) || (nullptr == m_downloadShader) || (nullptr == m_shader) ||
        (nullptr == m_uploadShader) || (nullptr == m_mixer) || (nullptr == m_convert) ||
        (nullptr == m_upload) || (nullptr == m_sink))
    {
        qDebug() << "ERROR: couldn't initiate pipeline!";
    }
}

void GStreamerWorker::createGstPipeline()
{
    createPipelineElements();

    GstCaps* caps = gst_caps_from_string("video/x-raw(memory:D3D11Memory), video/x-raw"); //
    g_object_set(G_OBJECT(m_capsfilter), "caps", caps, nullptr);
    gst_caps_unref(caps);

    g_object_set(m_delay,
                 "min-threshold-time", (guint64)m_msecDelay * ONE_MILLION,   // 50msec delay
                 "max-size-time", (guint64)200000000,
                 "max-size-buffers", (guint)0,
                 "max-size-bytes", (guint)0,
                 nullptr);

    g_object_set(m_sink, "sync", FALSE, "qos", FALSE, nullptr); // this switches off redundant synchronization

    gst_bin_add_many(GST_BIN(m_pipeline),
                     m_source,
                     m_download,
                     m_capsfilter,
                     m_tee,
                     m_origQueue,
                     m_delay,
                     m_uploadShader,
                     m_shader,
                     m_downloadShader,
                     m_mixer,
                     m_convert,
                     m_upload,
                     m_sink,
                     nullptr);

    // // working pipe
    // // d3d11screencapturesrc -> d3d11download -> videoconvert -> glupload -> qml6glsink
    // if (!gst_element_link_many(m_source, m_download, m_convert2, m_upload, m_sink, nullptr))
    // {
    //     qDebug() << "ERROR: couldn't link whole elements!";
    //     return;
    // }

    // link screen capture flow
    if (!gst_element_link_many(m_source, m_capsfilter, m_download, m_convert, m_tee, nullptr))
    {
        qDebug() << "ERROR: couldn't link capture flow pipeline elements!";
        return;
    }

    // link screen first flow
    if (!gst_element_link_many(m_tee, m_origQueue, m_mixer, nullptr))
    {
        qDebug() << "ERROR: couldn't link original flow pipeline elements!";
        return;
    }

    // link screen delayed flow
    if (!gst_element_link_many(m_tee, m_delay, m_uploadShader, m_shader, m_downloadShader, m_mixer, nullptr))
    {
        qDebug() << "ERROR: couldn't link delayed flow pipeline elements!";
        return;
    }

    // link screen output flow
    if (!gst_element_link_many(m_mixer, m_upload, m_sink, nullptr))
    {
        qDebug() << "ERROR: couldn't link output flow pipeline elements!";
        return;
    }

    // retrieve pads using names, GStreamer gives automatically during linking
    GstPad *sink0 = gst_element_get_static_pad(m_mixer, "sink_0");
    GstPad *sink1 = gst_element_get_static_pad(m_mixer, "sink_1");

    if (sink0 && sink1)
    {
        qDebug() << "Setting mixer pads properties...";

        g_object_set(sink0, "alpha", 1.0, "zorder", 1, nullptr);

        // sink1 - inverted upper layer
        // operator=ADD (2)
        // compositor will blend: Original + (Inverted)
        g_object_set(sink1, "alpha", 0.5, "zorder", 2, "operator", 2, nullptr);

        gst_pad_set_offset(sink1, (gint64)50000000);

        gst_object_unref(sink0);
        gst_object_unref(sink1);
    }
    else
    {
        qDebug() << "Pads not found! Try request_pad approach.";
    }
}

void GStreamerWorker::setPipelineProperties(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                        "width", G_TYPE_INT, width,
                                        "height", G_TYPE_INT, height,
                                        // "framerate", GST_TYPE_FRACTION, 30, 1, // set your FPS
                                        nullptr);

    g_object_set(G_OBJECT(m_capsfilter), "caps", caps, nullptr);

    gst_caps_unref(caps);
}

void GStreamerWorker::updateVideoFrameSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (!gst_element_set_state(m_pipeline, GST_STATE_PAUSED))
    {
        qDebug() << "ERROR: Could not pause pipeline!";
        handleGstError(m_pipeline);
    }

    // qDebug() << "x: " << x << " y: " << y << " width: " << width << " height: " << height;

    g_object_set(G_OBJECT(m_source),
                 "crop-x", x,
                 "crop-y", y,
                 "crop-width", width,
                 "crop-height", height,
                 nullptr);

    GstCaps* newCaps = gst_caps_new_simple("video/x-raw",
                                        "width", G_TYPE_INT, width,
                                        "height", G_TYPE_INT, height,
                                        nullptr);

    GstCapsFeatures* features = gst_caps_features_new("memory:D3D11Memory", nullptr);
    gst_caps_set_features(newCaps, 0, features);

    // Set new caps after resize
    g_object_set(G_OBJECT(m_capsfilter), "caps", newCaps, nullptr);
    gst_caps_unref(newCaps);

    // flush old frames
    gst_element_send_event(m_pipeline, gst_event_new_flush_start());
    gst_element_send_event(m_pipeline, gst_event_new_flush_stop(TRUE));

    if (!gst_element_set_state(m_pipeline, GST_STATE_PLAYING))
    {
        qDebug() << "ERROR: Could not start pipeline!";
        handleGstError(m_pipeline);
    }
}