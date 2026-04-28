#ifndef GSTREAMERWORKER_H
#define GSTREAMERWORKER_H

class GStreamerWorker
{
public:
    static GStreamerWorker& getInstance();
    void CreateGstPipeline();

private:
    GStreamerWorker();
    GStreamerWorker(const GStreamerWorker&) = delete;
    GStreamerWorker& operator=(const GStreamerWorker& instance) = delete;

    ~GStreamerWorker() = default;
};

#endif // GSTREAMERWORKER_H
