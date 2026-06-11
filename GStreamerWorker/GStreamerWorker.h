/**
 * @file    GStreamerWorker.h
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   GStreamerWorker do all work with gstreamer - initialization, configuring,
 *          builds pipeline, starts, stop, etc
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

#ifndef GSTREAMERWORKER_H
#define GSTREAMERWORKER_H

#include <gst/gst.h>
#include <stdint.h>

#include "BaseGStreamerWorker.h"

class QObject;

class GStreamerWorker : public BaseGStreamerWorker
{
public:
    static GStreamerWorker& getInstance();
    void createGstPipeline() override;
    GstElement* getSink() const;
    void setVideoSink(QObject* sink);
    uint16_t getDelayValue();
    void setDelayValue(uint16_t msecDelay);

    void updateVideoFrameSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    ~GStreamerWorker();

private:
    GStreamerWorker();
    GStreamerWorker(const GStreamerWorker&) = delete;
    GStreamerWorker& operator=(const GStreamerWorker& instance) = delete;

    void createPipelineElements() override;
    void setPipelineProperties(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

private:
    GstElement* m_pipeline;
    GstElement* m_source;
    GstElement* m_capsfilter;
    GstElement* m_download;
    GstElement* m_tee;          // video splitter
    GstElement* m_origQueue;
    GstElement* m_delay;
    GstElement* m_uploadShader;
    GstElement* m_shader;
    GstElement* m_downloadShader;
    GstElement* m_mixer;        // video compositor
    GstElement* m_convert;
    GstElement* m_upload;
    GstElement* m_sink;

    uint16_t m_msecDelay;
};

#endif // GSTREAMERWORKER_H
