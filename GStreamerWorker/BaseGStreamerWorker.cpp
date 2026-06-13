/**
 * @file    BaseGStreamerWorker.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   BaseGStreamerWorker base class to cover common operation
 *          with gstreamer functionality
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

const uint32_t ONE_MILLION = 1000000;

BaseGStreamerWorker::BaseGStreamerWorker()
{
    gst_init(nullptr, nullptr);
    m_msecDelay = 50;
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

uint16_t BaseGStreamerWorker::getDelayValue()
{
    return m_msecDelay;
}

void BaseGStreamerWorker::setDelayValue(uint16_t msecDelay)
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
