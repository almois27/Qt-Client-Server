#ifndef TCPSERVERTHREAD_H
#define TCPSERVERTHREAD_H


#include <QObject>
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
#include <QTcpSocket>

class QTcpServer;

using namespace tdv::nuitrack;

class TcpServerThread : public QThread
{
    Q_OBJECT
public:
    TcpServerThread(const char *server_ip, int server_port, const std::string path_to_oni);
    bool init();
    void initNuitrack();
    bool startNuitrack();
    void releaseNuitrack();
    const char* SERVER_IP;
    int SERVER_PORT;
    QByteArray data;
    const std::string pathToOni;


    void run();
    void Start();
    void Pause();

signals:
    void newSkeleton(QJsonDocument data);
public slots:
    void newConnection();
    void sockDisc();
    void onNewSkeletonData(QJsonDocument data);

private:
    SkeletonTracker::Ptr _skeletonTracker;
    QJsonArray m_joints;
    uint64_t _onIssuesUpdateHandler;
    bool _isInitialized;
    void onSkeletonUpdate(SkeletonData::Ptr userSkeletons);

    QTcpServer* m_pListener;
    QTcpSocket *socket;
};
#endif // TCPSERVERTHREAD_H
