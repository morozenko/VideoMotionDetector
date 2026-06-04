/**
 * @file    FilterWindowViewModel.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   VideoMotionDetectorViewModel responsible for properties in main view
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

#include "FilterWindowViewModel.h"

FilterWindowViewModel::FilterWindowViewModel(GStreamerWorker& worker)
    : m_X0(200),
      m_Y0(200),
      m_frameWidth(740),
      m_frameHeight(480),
      m_sliderValue(worker.getDelayValue()), // 50 msec
      m_worker(worker)
{
    connect(this, &FilterWindowViewModel::X0Changed,
            this, &FilterWindowViewModel::onSizePositionChanged);
    connect(this, &FilterWindowViewModel::Y0Changed,
            this, &FilterWindowViewModel::onSizePositionChanged);
    connect(this, &FilterWindowViewModel::widthChanged,
            this, &FilterWindowViewModel::onSizePositionChanged);
    connect(this, &FilterWindowViewModel::heightChanged,
            this, &FilterWindowViewModel::onSizePositionChanged);
}

uint16_t FilterWindowViewModel::getX0() const
{
    return m_X0;
}

uint16_t FilterWindowViewModel::getY0() const
{
    return m_Y0;
}

uint16_t FilterWindowViewModel::getWidth() const
{
    return m_frameWidth;
}

uint16_t FilterWindowViewModel::getHeight() const
{
    return m_frameHeight;
}

uint16_t FilterWindowViewModel::getSliderValue() const
{
    return m_sliderValue;
}

void FilterWindowViewModel::setX0(uint16_t X0)
{
    if (X0 != m_X0)
    {
        m_X0 = X0;
        emit X0Changed();
    }
}

void FilterWindowViewModel::setY0(uint16_t Y0)
{
    if (Y0 != m_Y0)
    {
        m_Y0 = Y0;
        emit Y0Changed();
    }
}

void FilterWindowViewModel::setWidth(uint16_t frameWidth)
{
    if (frameWidth != m_frameWidth)
    {
        m_frameWidth = frameWidth;
        emit widthChanged();
    }
}

void FilterWindowViewModel::setHeight(uint16_t frameHeight)
{
    if (frameHeight != m_frameHeight)
    {
        m_frameHeight = frameHeight;
        emit heightChanged();
    }
}

void FilterWindowViewModel::setSliderValue(uint16_t sliderValue)
{
    if (sliderValue != m_sliderValue)
    {
        m_sliderValue = sliderValue;
        m_worker.setDelayValue(sliderValue);
        emit sliderValueChanged();
    }
}

void FilterWindowViewModel::onSizePositionChanged()
{
    m_worker.updateVideoFrameSize(m_X0, m_Y0, m_frameWidth, m_frameHeight);
}