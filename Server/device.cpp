#include "device.h"

Device::Device()
{
   openni::Status st = openni::OpenNI::initialize();
   qDebug()<<"initialize status"<<st;
   if( st != openni::STATUS_OK)
   {
       qDebug()<< "Error initialize"<<openni::OpenNI::getExtendedError();
   }
}

Device::Device(const char *filePath)
{
    qDebug()<<"New device";
    openni::Status st = openni::OpenNI::initialize();
    qDebug()<<"initialize status"<<st;
    if( st != openni::STATUS_OK)
    {
        qDebug()<< "Error initialize"<<openni::OpenNI::getExtendedError();
    }

    qDebug()<<"filepath:"<<filePath;
    st = device.open(filePath);
    qDebug()<<"open divece status"<<st;
    if( st != openni::STATUS_OK)
    {
      qDebug()<< "Error open device"<<openni::OpenNI::getExtendedError();
    }

    pb = device.getPlaybackControl();
    if (device.hasSensor(openni::SENSOR_DEPTH)){

        st = depth.create (device, openni::SENSOR_DEPTH);
        qDebug()<< "Status depth steam create"<<st;

        depth.start ();
        qDebug()<< "Status depth start"<<st;
        st = depth.readFrame (&depthf);
        qDebug()<< "Status read depth frame"<<st;
    }
    else
    {
        qDebug()<< "No dethp sensor";
    }

    if(device.hasSensor(openni::SENSOR_COLOR))
    {
       st = color.create (device, openni::SENSOR_COLOR);
        qDebug()<< "Status color steam create"<<st;
        st = color.start ();
        qDebug()<< "Status color start"<<st;
        st = color.readFrame (&colorf);
        qDebug()<< "Status read color frame"<<st;
    }
    else
    {
        qDebug()<< "No color sensor";
    }

}

Device::~Device()
{
    color.stop();
    depth.stop();
    color.destroy();
    depth.destroy();
    device.close();
    openni::OpenNI::shutdown();
}

openni::Status Device::SteamsSeek()
{
    openni::Status st = openni::STATUS_ERROR;
    int changedIndex=-1;
    openni::VideoStream* streams[] = {&color, &depth};
    while(st != openni::STATUS_OK){
        st = openni::OpenNI::waitForAnyStream( streams, 2, &changedIndex, 0);
        if (st == openni::STATUS_OK)
        {
            switch (changedIndex)
            {
            case 0:
                color.readFrame(&colorf);
                break;
            case 1:
                depth.readFrame(&depthf);
                break;

            default:
                return openni::STATUS_ERROR;
            }
        }
    }
    return openni::STATUS_OK;
}

void Device::ImgFrames(QPixmap *  pixmapRGB, QPixmap *  pixmapDEPTH )
{
    QImage imageRGB,imageDEPTH;
    int imageWidth = 640;
    int imageHeight = 480;

    openni::Status st = openni::STATUS_OK;
    st = SteamsSeek();
    if(st != openni::STATUS_OK)
    {
        return;
    }

    uchar *data = (uchar *)(colorf.getData());
    imageRGB = QImage(data, imageWidth, imageHeight, QImage::Format_RGB888);
    *pixmapRGB = QPixmap::fromImage(imageRGB);

    data = (uchar *)(depthf.getData());
    imageDEPTH = QImage(data, imageWidth, imageHeight, QImage::Format_RGB16);
    *pixmapDEPTH = QPixmap::fromImage(imageDEPTH);
    return;
}

int Device::getNumberFrames()
{
    return pb->getNumberOfFrames(color);
}


void Device::SetFilePath(const char *newFilePath)
{
    this->filePath= newFilePath;
}


