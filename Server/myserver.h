#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <nuitrack/Nuitrack.h>
#include <iomanip>
#include <iostream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVector3D>
#include <QVector>
#include <QMap>
#include <QtConcurrent>
#include <QThreadPool>
#include <thread>

using namespace tdv::nuitrack;
class myServer: public QTcpServer
{
    Q_OBJECT
public:
    myServer(const char* server_ip = "127.0.0.1", int server_port = 1111, const std::string oni_file="");
    ~myServer();
    bool init();
    void initNuitrack();
    bool startNuitrack();
    void releaseNuitrack();

    const char* SERVER_IP;
    int SERVER_PORT;
    QTcpSocket * socket;
    QByteArray data;
    const std::string pathToOni;
signals:
    void newSkeleton(QJsonDocument data);
    void finished();
public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
    void onNewSkeletonData(QJsonDocument data);
    void isFinished();

private:
    SkeletonTracker::Ptr _skeletonTracker;
//    QMap <QString, QVector3D> m_joints;
    QJsonArray m_joints;
    uint64_t _onIssuesUpdateHandler;
    bool _isInitialized;
    void onSkeletonUpdate(SkeletonData::Ptr userSkeletons);
    std::thread m_thread;





};

#endif // MYSERVER_H
