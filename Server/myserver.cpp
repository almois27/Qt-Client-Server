#include "myserver.h"
myServer::myServer(const char *server_ip, int server_port, const std::string oni_file):
    SERVER_IP(server_ip),
    SERVER_PORT(server_port),
    pathToOni(oni_file),
    _isInitialized(false)
{
    std::cout << oni_file;
}

myServer::~myServer()
{

}

bool myServer::init()
{
     qDebug() << "is initializing " << "ip: " << SERVER_IP << ", port: " << SERVER_PORT << "\n";
     bool isInitialized = listen(QHostAddress(SERVER_IP), SERVER_PORT);
     if (isInitialized) {
              qDebug() << "is listening\n";
         }
     else {
            qDebug() << "is NOT listening\n";
        }
     return isInitialized;
}

void  myServer::initNuitrack()
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


    _skeletonTracker->connectOnUpdate(std::bind(&myServer::onSkeletonUpdate, this, std::placeholders::_1));

    // Start Nuitrack

    while(true){
         startNuitrack();
    }



}

bool myServer::startNuitrack()
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

void myServer::releaseNuitrack()
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


void myServer::onSkeletonUpdate(SkeletonData::Ptr userSkeletons)
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
                qMakePair("JOINT_HEAD"+QString::number(i), QJsonObject({  qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_HEAD].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_HEAD].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_HEAD].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));


            joint = QJsonObject(
            {
                qMakePair("JOINT_NECK"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_NECK].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_NECK].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_NECK].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_TORSO"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_TORSO].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_TORSO].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_TORSO].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_WAIST"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_WAIST].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_WAIST].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_WAIST].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_COLLAR"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_COLLAR].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_COLLAR].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_COLLAR].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_SHOULDER"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_SHOULDER].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_SHOULDER].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_SHOULDER].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_ELBOW"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_ELBOW].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_ELBOW].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_ELBOW].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_WRIST"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_WRIST].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_WRIST].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_WRIST].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_HAND"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_HAND].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_HAND].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_HAND].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_COLLAR"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_COLLAR].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_COLLAR].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_COLLAR].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_SHOULDER"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_SHOULDER].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_SHOULDER].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_SHOULDER].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_ELBOW"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_ELBOW].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_ELBOW].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_ELBOW].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_WRIST"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_WRIST].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_WRIST].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_WRIST].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_HAND"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_HAND].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_HAND].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_HAND].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_HIP"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_HIP].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_HIP].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_HIP].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_KNEE"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_KNEE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_KNEE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_KNEE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_LEFT_ANKLE"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_LEFT_ANKLE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_LEFT_ANKLE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_LEFT_ANKLE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));

            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_HIP"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_HIP].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_HIP].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_HIP].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_KNEE"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_KNEE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_KNEE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_KNEE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
            joint = QJsonObject(
            {
                qMakePair("JOINT_RIGHT_ANKLE"+QString::number(i), QJsonObject({
                                                                    qMakePair(QString("x"), QJsonValue(skeleton.joints[JOINT_RIGHT_ANKLE].proj.x)),
                                                                    qMakePair(QString("y"), QJsonValue(skeleton.joints[JOINT_RIGHT_ANKLE].proj.y)),
                                                                    qMakePair(QString("z"), QJsonValue(skeleton.joints[JOINT_RIGHT_ANKLE].proj.z))
                                                                }))
            });
            m_joints.push_back(QJsonValue(joint));
        i++;


    }
    QJsonDocument joints_data;
    joints_data.setArray(m_joints);
    emit newSkeleton(joints_data);


}


void myServer::startServer()
{

}

void myServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"incomingConnection";
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor((socketDescriptor));
    QFuture<void> future = QtConcurrent::run(std::bind(&myServer::initNuitrack, this));

    connect(socket,SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(this,SIGNAL(newSkeleton(QJsonDocument)), this, SLOT(onNewSkeletonData(QJsonDocument)));

}

void myServer::sockReady()
{
}

void myServer::sockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}

void myServer::onNewSkeletonData(QJsonDocument data)
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

void myServer::isFinished()
{
    if (socket->state() == QTcpSocket::ConnectedState)
        {
           socket->write("Finished");
        }
}
