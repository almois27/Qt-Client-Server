#include "tcpserverthread.h"
#include <iostream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>

TcpServerThread::TcpServerThread(const char *server_ip, int server_port, const std::string path_to_oni):
    SERVER_IP(server_ip),
    SERVER_PORT(server_port),
    pathToOni(path_to_oni),
    _isInitialized(false)
{

}

void TcpServerThread::run()
{
    std::cout << "thread started ..." << std::endl;
    std::cout << "listener starting ..." << std::endl;
    m_pListener = new QTcpServer();
    connect(m_pListener, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if(!m_pListener->listen(QHostAddress(SERVER_IP), SERVER_PORT))
    {
        std::cout << "listener could NOT START - " << m_pListener->errorString().toStdString() << std::endl;
    }
    else
    {
        std::cout << "listener SUCCESSFULLY STARTED" << std::endl;
    }
    exec();
}

void TcpServerThread::newConnection()
{
    qDebug() << "in new Connection ... ";
    socket = m_pListener->nextPendingConnection();
    QFuture<void> future = QtConcurrent::run(std::bind(&TcpServerThread::initNuitrack, this));

    connect(socket,SIGNAL(disconnected()), this, SLOT(sockDisc()),Qt::DirectConnection);
    connect(this,SIGNAL(newSkeleton(QJsonDocument)), this, SLOT(onNewSkeletonData(QJsonDocument)),Qt::DirectConnection);

}

void  TcpServerThread::initNuitrack()
{

    try
    {
        Nuitrack::init();
    }
    catch (const Exception& e)
    {
        std::cerr << "Can not initialize Nuitrack (ExceptionType: " << e.type() << ")" << std::endl;
        return;
    }



    Nuitrack::setConfigValue("OpenNIModule.FileRecord", pathToOni);


    _skeletonTracker = SkeletonTracker::create();


    _skeletonTracker->connectOnUpdate(std::bind(&TcpServerThread::onSkeletonUpdate, this, std::placeholders::_1));

    // Start Nuitrack
    while(true){
         startNuitrack();
    }



}

bool TcpServerThread::startNuitrack()
{
    if (!_isInitialized)
        {
            try
            {
                Nuitrack::run();
            }
            catch (const Exception& e)
            {
                std::cerr << "Can not start Nuitrack (ExceptionType: " << e.type() << ")" << std::endl;
                releaseNuitrack();
                exit(EXIT_FAILURE);
            }
            _isInitialized = true;
        }
        try
        {

            Nuitrack::waitUpdate(_skeletonTracker);
        }
        catch (const LicenseNotAcquiredException& e)
        {

            std::cerr << "LicenseNotAcquired exception (ExceptionType: " << e.type() << ")" << std::endl;
            return false;
        }
        catch (const Exception& e)
        {

            std::cerr << "Nuitrack update failed (ExceptionType: " << e.type() << ")" << std::endl;
            return false;
        }

        return true;

}

void TcpServerThread::releaseNuitrack()
{
       try
        {
            Nuitrack::release();
        }
        catch (const Exception& e)
        {
            std::cerr << "Nuitrack release failed (ExceptionType: " << e.type() << ")" << std::endl;
        }

        _isInitialized = false;

}

void TcpServerThread::onSkeletonUpdate(SkeletonData::Ptr userSkeletons)
{
    m_joints = QJsonArray();
    if (!userSkeletons)
    {

        std::cout << "No skeleyon data" << std::endl;
        return;
    }

    auto skeletons = userSkeletons->getSkeletons();
    int i = 1;
    for (auto skeleton: skeletons)
    {

            auto joint = QJsonObject(
            {
                qMakePair("JOINT_HEAD", QJsonObject({  qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_HEAD].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_HEAD].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_HEAD].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));


            joint = QJsonObject(
            {
                qMakePair("JOINT_NECK", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_NECK].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_NECK].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_NECK].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_TORSO", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_TORSO].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_TORSO].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_TORSO].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_WAIST", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_WAIST].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_WAIST].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_WAIST].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_COLLAR", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_COLLAR].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_COLLAR].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_COLLAR].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_SHOULDER", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_SHOULDER].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_SHOULDER].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_SHOULDER].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_ELBOW", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_ELBOW].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_ELBOW].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_ELBOW].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_WRIST", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_WRIST].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_WRIST].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_WRIST].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_HAND", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_HAND].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_HAND].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_HAND].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_COLLAR", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_COLLAR].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_COLLAR].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_COLLAR].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_SHOULDER", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_SHOULDER].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_SHOULDER].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_SHOULDER].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_ELBOW", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_ELBOW].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_ELBOW].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_ELBOW].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_WRIST", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_WRIST].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_WRIST].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_WRIST].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_HAND", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_HAND].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_HAND].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_HAND].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_HIP", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_HIP].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_HIP].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_HIP].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_KNEE", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_KNEE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_KNEE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_KNEE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_ANKLE", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_ANKLE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_ANKLE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_ANKLE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_HIP", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_HIP].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_HIP].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_HIP].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_KNEE", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_KNEE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_KNEE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_KNEE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_ANKLE", QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_ANKLE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_ANKLE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_ANKLE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            if(skeletons.size()>1)
            {
                break;
                //m_joints.push_back(QJsonValue('#'));
            }
            i++;

    }
    QJsonDocument joints_data;
    joints_data.setArray(m_joints);
    emit newSkeleton(joints_data);


}


void TcpServerThread::sockDisc()
{
    qDebug()<<"Disconnect";
    m_pListener->deleteLater();
}
void TcpServerThread::onNewSkeletonData(QJsonDocument data)
{

    if (socket->state() == QTcpSocket::ConnectedState)
        {
           qDebug()<<"new data";
           socket->write(QString::fromLatin1(data.toJson()).toLatin1());
           socket->waitForBytesWritten(1000);
           socket->write("*");
           socket->flush();
        }
}


void TcpServerThread::Start()
{
    start();
}

void TcpServerThread::Pause()
{

}
