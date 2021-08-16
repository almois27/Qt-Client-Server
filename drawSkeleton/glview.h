#ifndef GLVIEW_H
#define GLVIEW_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include <GL/glut.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMap>


class glView : public QOpenGLWidget
{
    Q_OBJECT
public:
    glView(QWidget  *parent = nullptr);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void setJointsMap(QMap<QString, QVector3D> joints_map);
signals:
    void GLupdate();

private:
    void drawJoints();
    void drawBones(int num);
    QMap <QString, QVector3D> m_joints;
    int number_skeleton;
    int number_joints = 20;
};

#endif // GLVIEW_H
