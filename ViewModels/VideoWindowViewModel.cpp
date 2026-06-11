#include <QCameraDevice>

#include "VideoWindowViewModel.h"

VideoWindowViewModel::VideoWindowViewModel(const VideoDeviceManager* videoDeviceManager)
    : m_cameraDevice(videoDeviceManager->getExternalVideoAdapter())
{
}

QCameraDevice* VideoWindowViewModel::cameraDevice()
{
    return m_cameraDevice;
}