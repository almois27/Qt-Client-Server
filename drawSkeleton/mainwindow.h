#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QBasicTimer>
#include <QTime>


#include "glview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket* socket;
    QByteArray data;
    QJsonDocument doc;
    QJsonParseError docError;

signals:
    void newSkeletonData(QByteArray  data);
public slots:
    void sockReady();
    void sockDisc();
    void onNewSkeletonData(QByteArray  data);

private slots:
    void on_connectButton_clicked();

private:
    Ui::MainWindow *ui;
    void parceJsonDocument(QJsonDocument joints_data);
    int SERVER_PORT;
};
#endif // MAINWINDOW_H
