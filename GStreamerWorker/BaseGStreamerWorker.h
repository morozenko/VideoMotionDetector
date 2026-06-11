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

class BaseGStreamerWorker
{
public:
    void startPlaying();

    virtual ~BaseGStreamerWorker();

protected:
    BaseGStreamerWorker();

    virtual void createGstPipeline() = 0;
    virtual void createPipelineElements() = 0;

    void handleGstError(GstElement* pipeline);

protected:
    GstElement* m_pipeline;
};

#endif // BASEGSTREAMERWORKER_H
