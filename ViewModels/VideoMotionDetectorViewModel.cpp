#include "VideoMotionDetectorViewModel.h"

VideoMotionDetectorViewModel::VideoMotionDetectorViewModel()
    : m_X0(200),
      m_Y0(200),
      m_frameWidth(640),
      m_frameHeight(480)
{
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