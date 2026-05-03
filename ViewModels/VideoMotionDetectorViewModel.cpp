#include "VideoMotionDetectorViewModel.h"

VideoMotionDetectorViewModel::VideoMotionDetectorViewModel(GStreamerWorker& worker)
    : m_X0(200),
      m_Y0(200),
      m_frameWidth(740),
      m_frameHeight(480),
      m_sliderValue(50), // 50 msec
      m_worker(worker)
{
    connect(this, &VideoMotionDetectorViewModel::X0Changed,
            this, &VideoMotionDetectorViewModel::onSizePositionChanged);
    connect(this, &VideoMotionDetectorViewModel::Y0Changed,
            this, &VideoMotionDetectorViewModel::onSizePositionChanged);
    connect(this, &VideoMotionDetectorViewModel::widthChanged,
            this, &VideoMotionDetectorViewModel::onSizePositionChanged);
    connect(this, &VideoMotionDetectorViewModel::heightChanged,
            this, &VideoMotionDetectorViewModel::onSizePositionChanged);
}

uint16_t VideoMotionDetectorViewModel::getX0() const
{
    return m_X0;
}

uint16_t VideoMotionDetectorViewModel::getY0() const
{
    return m_Y0;
}

uint16_t VideoMotionDetectorViewModel::getWidth() const
{
    return m_frameWidth;
}

uint16_t VideoMotionDetectorViewModel::getHeight() const
{
    return m_frameHeight;
}

uint16_t VideoMotionDetectorViewModel::getSliderValue() const
{
    return m_sliderValue;
}

void VideoMotionDetectorViewModel::setX0(uint16_t X0)
{
    if (X0 != m_X0)
    {
        m_X0 = X0;
        emit X0Changed();
    }
}

void VideoMotionDetectorViewModel::setY0(uint16_t Y0)
{
    if (Y0 != m_Y0)
    {
        m_Y0 = Y0;
        emit Y0Changed();
    }
}

void VideoMotionDetectorViewModel::setWidth(uint16_t frameWidth)
{
    if (frameWidth != m_frameWidth)
    {
        m_frameWidth = frameWidth;
        emit widthChanged();
    }
}

void VideoMotionDetectorViewModel::setHeight(uint16_t frameHeight)
{
    if (frameHeight != m_frameHeight)
    {
        m_frameHeight = frameHeight;
        emit heightChanged();
    }
}

void VideoMotionDetectorViewModel::setSliderValue(uint16_t sliderValue)
{
    if (sliderValue != m_sliderValue)
    {
        m_sliderValue = sliderValue;
        emit sliderValueChanged();
    }
}

void VideoMotionDetectorViewModel::onSizePositionChanged()
{
    m_worker.updateVideoFrameSize(m_X0, m_Y0, m_frameWidth, m_frameHeight);
}