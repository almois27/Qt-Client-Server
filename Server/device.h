#ifndef DEVICE_H
#define DEVICE_H



#include <QObject>
#include <nuitrack/Nuitrack.h>
using namespace tdv::nuitrack;

class Device:public QObject
{
    Q_OBJECT
public:
    Device();
    ~Device();

    int startNuitrack();

    static void onSkeletonUpdate(SkeletonData::Ptr userSkeletons);
    static void getSkeleton(const std::vector<Joint>& joints);


    void ImgFrames(QPixmap *  pixmapRGB, QPixmap *  pixmapDEPTH );
    int getNumberFrames();
    void SetFilePath(const char* newFilePath);
    openni::Status SteamsSeek();


private:
    const char* filePath;
    openni::Device device;
    openni::VideoStream color;
    openni::VideoFrameRef colorf;
    openni::VideoStream depth;
    openni::VideoFrameRef depthf;
    openni::PlaybackControl * pb;

    enum play_state_type playState = STOP;



};

#endif // DEVICE_H
