/**
 * @file    ExternalVideoGStreamerWorker.h
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   GStreamerWorker do all work with gstreamer - initialization, configuring,
 *          builds pipeline, starts, stop, etc
 *          Video is captured from external video adapter. Then two flows are blended.
 * @version 1.0
 * @date    2026-06-13
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

#ifndef EXTERNALVIDEOGSTREAMERWORKER_H
#define EXTERNALVIDEOGSTREAMERWORKER_H

#include <gst/gst.h>
#include <stdint.h>

#include "BaseGStreamerWorker.h"

class QObject;

class ExternalVideoGStreamerWorker : public BaseGStreamerWorker
{
public:
    static ExternalVideoGStreamerWorker& getInstance();
    void createGstPipeline(const QCameraDevice* cameraDevice = nullptr) override;
    GstElement* getSink() const;
    void setVideoSink(QObject* sink);
    uint16_t getDelayValue();
    void setDelayValue(uint16_t msecDelay);

    void updateVideoFrameSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;

    ~ExternalVideoGStreamerWorker();

private:
    ExternalVideoGStreamerWorker();
    ExternalVideoGStreamerWorker(const ExternalVideoGStreamerWorker&) = delete;
    ExternalVideoGStreamerWorker& operator=(const ExternalVideoGStreamerWorker& instance) = delete;

    void createPipelineElements();
    void setPipelineProperties(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

private:
    GstElement* m_source;
    GstElement* m_capsfilter;
    GstElement* m_decode;
    GstElement* m_videoconvert;
    GstElement* m_tee;          // video splitter
    GstElement* m_origQueue;
    GstElement* m_delay;
    GstElement* m_uploadShader;
    GstElement* m_shader;
    GstElement* m_downloadShader;
    GstElement* m_convert;
    GstElement* m_upload;
};

#endif // EXTERNALVIDEOGSTREAMERWORKER_H
