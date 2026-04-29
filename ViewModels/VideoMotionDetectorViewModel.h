#ifndef VIDEOMOTIONDETECTORVIEWMODEL_H
#define VIDEOMOTIONDETECTORVIEWMODEL_H

#include <QObject>

class VideoMotionDetectorViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(uint16_t X0 READ getX0 WRITE setX0 NOTIFY X0Changed)
    Q_PROPERTY(uint16_t Y0 READ getY0 WRITE setY0 NOTIFY Y0Changed)
    Q_PROPERTY(uint16_t frameWidth READ getWidth WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(uint16_t frameHeight READ getHeight WRITE setHeight NOTIFY heightChanged)

public:
    VideoMotionDetectorViewModel();
    ~VideoMotionDetectorViewModel() = default;

    // getters won't be used I believe
    uint16_t getX0() const;
    uint16_t getY0() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;

    void setX0(uint16_t X0);
    void setY0(uint16_t Y0);
    void setWidth(uint16_t frameWidth);
    void setHeight(uint16_t frameHeight);

signals:
    void X0Changed();
    void Y0Changed();
    void widthChanged();
    void heightChanged();

private:
    uint16_t m_X0;
    uint16_t m_Y0;
    uint16_t m_frameWidth;
    uint16_t m_frameHeight;
};

#endif // VIDEOMOTIONDETECTORVIEWMODEL_H
