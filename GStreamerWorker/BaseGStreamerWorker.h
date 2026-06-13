/**
 * @file    BaseGStreamerWorker.h
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   BaseGStreamerWorker interface which handles common functionality
 *          to work with gstreamer - initialization, pipeline start/stop.
 * @version 1.0
 * @date    2026-06-11
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

#ifndef BASEGSTREAMERWORKER_H
#define BASEGSTREAMERWORKER_H

#include <gst/gst.h>

class QCameraDevice;

class BaseGStreamerWorker
{
public:
    void startPlaying();
    uint16_t getDelayValue();
    void setDelayValue(uint16_t msecDelay);

    virtual void updateVideoFrameSize(uint16_t x, uint16_t y, uint16_t width, uint16_t height) = 0;

    virtual ~BaseGStreamerWorker();

protected:
    BaseGStreamerWorker();

    virtual void createGstPipeline(const QCameraDevice* cameraDevice = nullptr) = 0;

    void handleGstError(GstElement* pipeline);

protected:
    GstElement* m_pipeline;
    GstElement* m_sink;
    GstElement* m_mixer;        // video compositor

    uint16_t m_msecDelay;
};

#endif // BASEGSTREAMERWORKER_H
