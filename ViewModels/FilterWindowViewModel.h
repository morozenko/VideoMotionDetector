/**
 * @file    FilterWindowViewModel.h
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

#ifndef FILTERWINDOWVIEWMODEL_H
#define FILTERWINDOWVIEWMODEL_H

#include <QObject>

#include "GStreamerWorker/BaseGStreamerWorker.h"

class FilterWindowViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint16_t X0 READ getX0 WRITE setX0 NOTIFY X0Changed)
    Q_PROPERTY(uint16_t Y0 READ getY0 WRITE setY0 NOTIFY Y0Changed)
    Q_PROPERTY(uint16_t frameWidth READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(uint16_t frameHeight READ getHeight WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(uint16_t sliderValue READ getSliderValue WRITE setSliderValue NOTIFY sliderValueChanged)

public:
    FilterWindowViewModel(BaseGStreamerWorker* worker);
    ~FilterWindowViewModel() = default;

    uint16_t getX0() const;
    uint16_t getY0() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    uint16_t getSliderValue() const;

    void setX0(uint16_t X0);
    void setY0(uint16_t Y0);
    void setWidth(uint16_t frameWidth);
    void setHeight(uint16_t frameHeight);
    void setSliderValue(uint16_t sliderValue);

signals:
    void X0Changed();
    void Y0Changed();
    void widthChanged();
    void heightChanged();
    void sliderValueChanged();

private slots:
    void onSizePositionChanged();

private:
    uint16_t m_X0;
    uint16_t m_Y0;
    uint16_t m_frameWidth;
    uint16_t m_frameHeight;
    uint16_t m_sliderValue; // miliseconds

    BaseGStreamerWorker* m_worker;
};

#endif // FILTERWINDOWVIEWMODEL_H
