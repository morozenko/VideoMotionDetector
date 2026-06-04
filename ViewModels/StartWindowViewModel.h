/**
 * @file    StartWindowViewModel.h
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   VideoMotionDetectorViewModel responsible for properties in starting view
 *          In constructor it receives VideoDeviceManager, which knows whether
 *          external usb device is connected
 *
 * @version 1.0
 * @date    2026-06-04
 *
 * @copyright Andrii Moroz (c) 2026
 * All rights reserved
 *
 * WARNING: This code is confidential and proprietary.
 * Unauthorized copying, distribution, or use of this file,
 * in any medium, is strictly prohibited without author permission.
 */

#ifndef STARTWINDOWVIEWMODEL_H
#define STARTWINDOWVIEWMODEL_H

#include <QObject>

#include "VideoManager/VideoDeviceManager.h"

class StartWindowViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool usbVideoEnbled READ usbVideoEnbled NOTIFY usbVideoEnbledChanged)
    Q_PROPERTY(uint8_t screensAmount READ screensAmount NOTIFY screensAmountChanged)

public:
    StartWindowViewModel(const VideoDeviceManager& videoDeviceManager);

    bool usbVideoEnbled();
    uint8_t screensAmount();

    Q_INVOKABLE void onFilterButtonClicked();

signals:
    void usbVideoEnbledChanged();
    void screensAmountChanged();

    void goToFilterWindow();

private:
    bool m_usbVideoEnabled;
    uint8_t m_screensAmount;
};

#endif // STARTWINDOWVIEWMODEL_H
