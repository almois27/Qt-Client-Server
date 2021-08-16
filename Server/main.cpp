#include <QCoreApplication>
#include <QThread>
#include "tcpserverthread.h"
#include "myserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Set directory to ONI file: " << Qt::endl;
    QTextStream cin(stdin);
    QString path_to_oni;
    cin>>path_to_oni;

    myServer* server = new myServer("127.0.0.1", 1111, path_to_oni.toStdString());
    server->init();

//    myServer* server2 = new myServer("127.0.0.1", 5555, "D:/Qt6/Projects/Test/Server/cap2.oni");
//    QThread thread2;
//    server2->moveToThread(&thread2);
//    server2->init();


//    TcpServerThread *t1 = new TcpServerThread("127.0.0.1", 1111, "D:/Qt6/Projects/Test/Server/cap1.oni");
//    t1->Start();

//    TcpServerThread *t2 = new TcpServerThread("127.0.0.1", 5555, "D:/Qt6/Projects/Test/Server/cap2.oni");
//    t2->Start();

    return a.exec();
}

