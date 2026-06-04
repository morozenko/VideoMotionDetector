/**
 * @file    VideoDeviceManager.cpp
 * @author  Andrii Moroz (andriimoroz88@gmail.com)
 * @brief   VideoDeviceManager responsible for external usb video device management
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

#include <QMediaDevices>
#include <QDebug>

#include "VideoDeviceManager.h"

VideoDeviceManager::VideoDeviceManager()
    : m_usbDevice(nullptr)
{}

QCameraDevice* VideoDeviceManager::getExternalVideoAdapter() const
{
    if (m_usbDevice)
    {
        return m_usbDevice.get();
    }

    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

    for (const QCameraDevice& camera : cameras)
    {
        QString description = camera.description().toLower();
        qDebug() << "Found device: " << camera.description();

        // Choose proper device by name
        if (description.contains("usb video"))
        {
            qDebug() << "Choose for capturing: " << camera.description();
            m_usbDevice = std::make_unique<QCameraDevice>(camera);

            return m_usbDevice.get();
        }
    }

    return nullptr;
}