#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    socket =new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(this, SIGNAL(newSkeletonData(QByteArray)), this, SLOT(onNewSkeletonData(QByteArray)));


    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sockReady()
{
    QByteArray data;
    ui->label->setText("waitForConnected");
    if(socket->waitForConnected((500)))
    {
        ui->label->setText("Connected");
        if (socket->waitForReadyRead(300)){
          data.append(socket->readAll());
          if (data.right(1)[0] == '*')
           {
               emit newSkeletonData(data);
               data.clear();
           }

        }

    }
    else{
        ui->label->setText("disonnected");
    }


}

void MainWindow::sockDisc()
{
    socket->deleteLater();
}



void MainWindow::onNewSkeletonData(QByteArray data)
{
    data = data.split('*')[0];


   doc = QJsonDocument::fromJson(data, &docError);

    if(docError.errorString().toInt() == QJsonParseError::NoError){

       parceJsonDocument(doc);
    }
    else
    {
        qDebug()<<docError.errorString();

    }
}

void MainWindow::parceJsonDocument(QJsonDocument joints_data)
{
    QMap <QString, QVector3D> map_joints;
    QJsonArray array = joints_data.array();
    if(array.empty()){
        return;
    }

    for(auto joint_obj : array)
    {
        auto joint_names = joint_obj.toObject().keys();
        if(joint_names.empty()){
            return;
        }

        for(auto jn: joint_names)
        {
           auto xyz = joint_obj.toObject().value(jn);

           double x = xyz.toObject().value("x").toDouble();
           double y = xyz.toObject().value("y").toDouble();
           double z = xyz.toObject().value("z").toDouble();

           map_joints[jn]=QVector3D(x,y,z);


        }

    }

     ui->widgetogl->setJointsMap(map_joints);
     repaint();


}
void MainWindow::on_connectButton_clicked()
{

    socket->connectToHost("127.0.0.1", 1111);
}


