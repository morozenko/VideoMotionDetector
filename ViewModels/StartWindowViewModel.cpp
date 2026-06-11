/**
 * @file    StartWindowViewModel.cpp
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

#include "StartWindowViewModel.h"

StartWindowViewModel::StartWindowViewModel(const VideoDeviceManager* videoDeviceManager)
    : m_usbVideoEnabled(false),
      m_screensAmount(0)
{
    if (nullptr != videoDeviceManager->getExternalVideoAdapter())
    {
        m_usbVideoEnabled = true;
    }

    m_screensAmount = QGuiApplication::screens().size();

    qDebug() << "Video adapter enabled: " << m_usbVideoEnabled;
    qDebug() << "Screens amount: " << m_screensAmount;
}

bool StartWindowViewModel::usbVideoEnbled()
{
    return m_usbVideoEnabled;
}

uint8_t StartWindowViewModel::screensAmount()
{
    return m_screensAmount;
}

void StartWindowViewModel::onFilterButtonClicked()
{
    emit goToFilterWindow();
}

void StartWindowViewModel::onDualViewButtonClicked()
{
    emit goToDualView();
}