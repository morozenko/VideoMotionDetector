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

#include "BaseGStreamerWorker.h"

BaseGStreamerWorker::BaseGStreamerWorker()
{
    gst_init(nullptr, nullptr);
}

BaseGStreamerWorker::~BaseGStreamerWorker()
{
    gst_object_unref(m_pipeline); // should unref all elemnts in pipeline too
}

void BaseGStreamerWorker::startPlaying()
{
    // start pipeline
    auto ret = gst_element_set_state(m_pipeline, GST_STATE_PLAYING);

    if (GST_STATE_CHANGE_FAILURE == ret)
    {
        qDebug() << "ERROR: cant change state to PLAYING!";
        handleGstError(m_pipeline);
        return;
    }
}

void BaseGStreamerWorker::handleGstError(GstElement* pipeline)
{
    GstBus* bus = gst_element_get_bus(pipeline);

    // wait for error message or EOS
    GstMessage* msg = gst_bus_timed_pop_filtered(
        bus,
        GST_CLOCK_TIME_NONE,
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
        );

    if (msg != nullptr)
    {
        if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR)
        {
            GError* err = nullptr;
            gchar* debug_info = nullptr;

            gst_message_parse_error(msg, &err, &debug_info);

            qDebug() << "Error from element: " << GST_OBJECT_NAME(msg->src);
            qDebug() << "Message: " << err->message;
            if (debug_info)
            {
                qDebug() << "Debug info: " << debug_info;
            }

            g_error_free(err);
            g_free(debug_info);
        }
        else if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_EOS)
        {
            qDebug() << "(End-Of-Stream).";
        }
        gst_message_unref(msg);
    }

    gst_object_unref(bus);
}
