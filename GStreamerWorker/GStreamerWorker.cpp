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

void GStreamerWorker::createPipelineElements()
{
    m_pipeline = gst_pipeline_new("screen-capture-pipeline");
    m_source = gst_element_factory_make("d3d11screencapturesrc", "source"); //d3d11screencapturesrc
    m_capsfilter = gst_element_factory_make("capsfilter", "filter");
    m_capsfilter2 = gst_element_factory_make("capsfilter", "filter2");
    m_download = gst_element_factory_make("d3d11download", "download");
    m_tee = gst_element_factory_make("tee", "tee");
    m_origQueue = gst_element_factory_make("queue", "origQueue");
    m_delayedQueue = gst_element_factory_make("queue", "delayedQueue");
    m_delay = gst_element_factory_make("queue", "delay");
    m_invert = gst_element_factory_make("frei0r-filter-invert0r", "invert");
    m_alpha = gst_element_factory_make("alpha", "alpha");
    m_alpha2 = gst_element_factory_make("alpha", "alpha2");
    m_mixer = gst_element_factory_make("compositor", "mixer");
    m_convert = gst_element_factory_make("videoconvert", "convert"); // glcolorconvert
    m_upload = gst_element_factory_make("glupload", "upload");
    m_sink = gst_element_factory_make("qml6glsink", "mysink");

    if ((nullptr == m_pipeline) || (nullptr == m_source) || (nullptr == m_capsfilter) || (nullptr == m_capsfilter2) ||
        (nullptr == m_download) || (nullptr == m_tee) || (nullptr == m_origQueue) ||
        (nullptr == m_delayedQueue) || (nullptr == m_delay) || (nullptr == m_invert) ||
        (nullptr == m_alpha) || (nullptr == m_alpha) || (nullptr == m_mixer) ||
        (nullptr == m_convert) || (nullptr == m_upload) || (nullptr == m_sink))
    {
        qDebug() << "ERROR: couldn't initiate pipeline!";
    }
}

void GStreamerWorker::CreateGstPipeline()
{
    createPipelineElements();

    GstCaps* caps = gst_caps_from_string("video/x-raw(memory:D3D11Memory), framerate=60/1"); //
    g_object_set(G_OBJECT(m_capsfilter), "caps", caps, nullptr);
    g_object_set(G_OBJECT(m_capsfilter2), "caps", caps, nullptr);
    gst_caps_unref(caps);

    // g_object_set(m_invert, "contrast", -1.0, "brightness", 1.0, nullptr);
    g_object_set(m_alpha, "alpha", 0.5, nullptr); // alpha for orig pipeline
    g_object_set(m_alpha2, "alpha", 0.5, nullptr); // alpha for delayed pipeline
    g_object_set(m_delay,
                 "min-threshold-time", (guint64)50000000,   // 50msec delay
                 "max-size-time", (guint64)200000000,
                 "max-size-buffers", (guint)0,
                 "max-size-bytes", (guint)0,
                 nullptr);

    GstElement* m_convert1 = gst_element_factory_make("d3d11convert", "conv_orig"); // d3d11colorconvert
    GstElement* m_convert2 = gst_element_factory_make("videoconvert", "conv_delayed");
    GstElement *m_offset = gst_element_factory_make("identity", "offset_identity");
    g_object_set(m_offset, "ts-offset", (gint64)50000000, nullptr); // +50мс до мітки часу

    g_object_set(m_sink, "sync", FALSE, "qos", FALSE, nullptr);
    g_object_set(G_OBJECT(m_mixer), "latency", (guint64)50000000, nullptr); // 100ms
    g_object_set(m_mixer, "start-deadline", (guint64)0, nullptr);

    // Не забудьте перевірити на nullptr разом з іншими
    if (!m_convert1 || !m_convert2) {
        qDebug() << "ERROR: couldn't create converters!";
    }


    gst_bin_add_many(GST_BIN(m_pipeline),
                     m_source,
                     m_download,
                     m_capsfilter,
                     m_capsfilter2,
                     m_tee,
                     m_origQueue,
                     m_delayedQueue,
                     m_delay,
                     m_convert1,
                     m_convert2,
                     m_offset,
                     m_invert,
                     m_alpha,
                     m_alpha2,
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
    if (!gst_element_link_many(m_tee, m_origQueue, /*m_convert1, m_capsfilter, m_alpha,*/ m_mixer, nullptr))
    {
        qDebug() << "ERROR: couldn't link original flow pipeline elements!";
        return;
    }

    // link screen delayed flow
    if (!gst_element_link_many(m_tee, /*m_delayedQueue,*/ m_delay, m_offset, m_invert, /*m_alpha2,*/ m_mixer, nullptr))
    {
        qDebug() << "ERROR: couldn't link delayed flow pipeline elements!";
        return;
    }

    // GstCaps* ram_caps = gst_caps_from_string("video/x-raw, format=RGBA");

    // gst_element_link_many(m_tee, m_delayedQueue, m_delay, /*m_convert2,*/ nullptr);

    // // 2. Від Convert до Invert (примусово RGBA)
    // if (!gst_element_link_filtered(m_delay, /*m_convert2,*/ m_invert, ram_caps)) {
    //     qDebug() << "Link failed: convert2 to frei0r (RGBA expected)";
    // }

    // // 3. Від Invert до Mixer (також через RGBA для безпеки)
    // if (!gst_element_link_filtered(m_invert, m_mixer, ram_caps)) {
    //     qDebug() << "Link failed: frei0r to mixer";
    // }
    // gst_caps_unref(ram_caps);

    // link screen output flow
    if (!gst_element_link_many(m_mixer, /*m_convert,*/ m_upload, m_sink, nullptr))
    {
        qDebug() << "ERROR: couldn't link output flow pipeline elements!";
        return;
    }

    // // Отримуємо пад міксера, до якого підключена затримана гілка
    // GstPad *mixer_delayed_pad = gst_element_get_static_pad(m_invert, "src");
    // // Це не зовсім вірно, треба брати SINK пад міксера:
    // GstPad *sink_pad_delayed = gst_element_get_compatible_pad(m_mixer, mixer_delayed_pad, nullptr);

    // // Встановлюємо зміщення часу для цього паду (50 мс)
    // g_object_set(sink_pad_delayed, "offsets", (gint64)50000000, nullptr);
    // gst_object_unref(sink_pad_delayed);
    // gst_object_unref(mixer_delayed_pad);

    // Спробуємо отримати пади за іменами, які GStreamer дає автоматично при лінкуванні
    GstPad *sink0 = gst_element_get_static_pad(m_mixer, "sink_0");
    GstPad *sink1 = gst_element_get_static_pad(m_mixer, "sink_1");

    if (sink0 && sink1)
    {
        qDebug() << "Setting mixer pads properties...";
        // Обидва мають бути напівпрозорими для ефекту зникнення фону
        // g_object_set(sink0, "alpha", 0.5, "zorder", 1, nullptr);
        // g_object_set(sink1, "alpha", 0.5, "zorder", 2, nullptr);

        g_object_set(sink0, "alpha", 1.0, "zorder", 1, nullptr);

        // sink1 - inverted upper layer
        // operator=ADD (2)
        // Це змусить compositor складати значення пікселів: Original + (Inverted)
        g_object_set(sink1, "alpha", 0.5, "zorder", 2, "operator", 2, nullptr);

        g_object_set(sink1, "offsets", (gint64)50000000, nullptr);

        gst_object_unref(sink0);
        gst_object_unref(sink1);
    }
    else
    {
        qDebug() << "Pads not found! Try request_pad approach.";
    }


    // if (!gst_element_link_many(m_source,
    //                            // m_capsfilter,
    //                            m_download,
    //                            m_invert,
    //                            m_alpha,
    //                            m_convert,
    //                            m_upload,
    //                            m_sink,
    //                            nullptr))
    // {
    //     qDebug() << "ERROR: couldn't link pipeline elements!";
    //     return;
    // }
}

void GStreamerWorker::setPipelineProperties(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    // g_object_set(G_OBJECT(m_videocrop),
    //              "top", y,
    //              "left", x,
    //              "right", 1920 - (x + width),
    //              "bottom", 1080 - (y + height),
    //              nullptr);

    GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                        "width", G_TYPE_INT, width,
                                        "height", G_TYPE_INT, height,
                                        // "framerate", GST_TYPE_FRACTION, 30, 1, // set your FPS
                                        nullptr);

    g_object_set(G_OBJECT(m_capsfilter), "caps", caps, nullptr);

    gst_caps_unref(caps);
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

void GStreamerWorker::updateVideoFrameSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if (!gst_element_set_state(m_pipeline, GST_STATE_PAUSED))
    {
        qDebug() << "ERROR: Could not pause pipeline!";
    }

    // qDebug() << "x: " << x << " y: " << y << " width: " << width << " height: " << height;

    g_object_set(G_OBJECT(m_source),
                 "crop-x", x,
                 "crop-y", y,
                 "crop-width", width,
                 "crop-height", height,
                 nullptr);

    setPipelineProperties(x, y, width, height);

    if (!gst_element_set_state(m_pipeline, GST_STATE_PLAYING))
    {
        qDebug() << "ERROR: Could not start pipeline!";
    }
}