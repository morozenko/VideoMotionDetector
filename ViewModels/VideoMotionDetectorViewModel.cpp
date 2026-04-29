#include "VideoMotionDetectorViewModel.h"

VideoMotionDetectorViewModel::VideoMotionDetectorViewModel(GStreamerWorker& worker)
    : m_Worker(worker)
{
}

QObject* VideoMotionDetectorViewModel::videoSink() const
{
    return nullptr;//m_Worker.getSink();
}

void VideoMotionDetectorViewModel::setVideoSink(QObject* sink)
{
    m_Worker.setVideoSink(sink);
    emit videoSinkChanged();
}