#ifndef VIDEOWINDOWVIEWMODEL_H
#define VIDEOWINDOWVIEWMODEL_H

#include <QObject>

#include <VideoManager/VideoDeviceManager.h>

class QCameraDevice;

class VideoWindowViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QCameraDevice* cameraDevice READ cameraDevice NOTIFY cameraDeviceChanged FINAL)

public:
    VideoWindowViewModel(const VideoDeviceManager* videoDeviceManager);
    QCameraDevice* cameraDevice();

signals:
    void cameraDeviceChanged();

private:
    QCameraDevice* m_cameraDevice;
};

#endif // VIDEOWINDOWVIEWMODEL_H
