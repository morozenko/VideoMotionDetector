#ifndef VIDEOMOTIONDETECTORVIEWMODEL_H
#define VIDEOMOTIONDETECTORVIEWMODEL_H

#include <QObject>

#include "GStreamerWorker/GStreamerWorker.h"

class VideoMotionDetectorViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)

public:
    VideoMotionDetectorViewModel(GStreamerWorker& worker);
    ~VideoMotionDetectorViewModel() = default;

    QObject* videoSink() const;

    void setVideoSink(QObject* sink);

signals:
    void videoSinkChanged();

private:
    GStreamerWorker& m_Worker;
};

#endif // VIDEOMOTIONDETECTORVIEWMODEL_H
