/**
 * @file    VideoDeviceManager.h
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

#ifndef VIDEODEVICEMANAGER_H
#define VIDEODEVICEMANAGER_H

#include <memory>
#include <QCameraDevice>

class VideoDeviceManager
{
public:
    VideoDeviceManager();

    QCameraDevice* getExternalVideoAdapter() const;

private:
    mutable std::unique_ptr<QCameraDevice> m_usbDevice;
};

#endif // VIDEODEVICEMANAGER_H
