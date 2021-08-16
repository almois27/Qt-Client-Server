#ifndef ONIPLAYER_H
#define ONIPLAYER_H


#include <QString>
#include <QPixmap>
//#include <OpenNI.h>
#include <QDebug>

enum play_state_type{STOP=0,PLAY,PAUSE};

class OniPlayer
{
public:
    OniPlayer();
//    OniPlayer(const char* filePath);
    ~OniPlayer();

//    void ImgFrames(QPixmap *pixmapRGB, QPixmap *pixmapDEPTH);
//    int getNumberFrames();
//    void SetFilePath(const char* newFilePath);
//    openni::Status SteamsSeek();


private:
//    const char* filePath;
//    openni::Device device;
//    openni::VideoStream color;
//    openni::VideoFrameRef colorf;
//    openni::VideoStream depth;
//    openni::VideoFrameRef depthf;
//    openni::PlaybackControl * pb;

//    enum play_state_type playState = STOP;



};


#endif // ONIPLAYER_H
