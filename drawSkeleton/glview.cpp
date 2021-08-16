#include "glview.h"
glView::glView(QWidget  *parent)
    : QOpenGLWidget(parent)
{
}

void glView::initializeGL()
{
    glClearColor(1,1,1,1);
}

void glView::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void glView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(!m_joints.empty())
    {
        drawJoints();
        for(int i=1;i<=number_skeleton;i++)
        {
            drawBones(i);
        }

    }
    emit GLupdate();

}

void glView::drawJoints()
{
    glColor3d(0.41,0.34,0.8);
    glPointSize(5);
    glBegin(GL_POINTS);
    for(auto v : m_joints)
    {
       glVertex3d(1-v.x(),1-v.y(),0);
    }
    glEnd();
}
void glView::drawBones(int num)
{
    glColor3d(0,0.54,0.54);
    glLineWidth(2) ;
    glBegin(GL_LINES);
        glVertex3d(1-m_joints["JOINT_HEAD"+QString::number(num)].x(),1-m_joints["JOINT_HEAD"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_NECK"+QString::number(num)].x(),1-m_joints["JOINT_NECK"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_NECK"+QString::number(num)].x(),1-m_joints["JOINT_NECK"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_TORSO"+QString::number(num)].x(),1-m_joints["JOINT_TORSO"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_SHOULDER"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_SHOULDER"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_COLLAR"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_SHOULDER"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_SHOULDER"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_WAIST"+QString::number(num)].x(),1-m_joints["JOINT_WAIST"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_HIP"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_HIP"+QString::number(num)].y(),0);


        glVertex3d(1-m_joints["JOINT_WAIST"+QString::number(num)].x(),1-m_joints["JOINT_WAIST"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_HIP"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_HIP"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_TORSO"+QString::number(num)].x(),1-m_joints["JOINT_TORSO"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_WAIST"+QString::number(num)].x(),1-m_joints["JOINT_WAIST"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_SHOULDER"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_SHOULDER"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_ELBOW"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_ELBOW"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_ELBOW"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_ELBOW"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_WRIST"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_WRIST"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_WRIST"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_WRIST"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_HAND"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_HAND"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_RIGHT_SHOULDER"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_SHOULDER"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_ELBOW"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_ELBOW"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_RIGHT_ELBOW"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_ELBOW"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_WRIST"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_WRIST"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_RIGHT_WRIST"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_WRIST"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_HAND"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_HAND"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_RIGHT_HIP"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_HIP"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_KNEE"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_KNEE"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_HIP"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_HIP"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_KNEE"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_KNEE"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_RIGHT_KNEE"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_KNEE"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_RIGHT_ANKLE"+QString::number(num)].x(),1-m_joints["JOINT_RIGHT_ANKLE"+QString::number(num)].y(),0);

        glVertex3d(1-m_joints["JOINT_LEFT_KNEE"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_KNEE"+QString::number(num)].y(),0);
        glVertex3d(1-m_joints["JOINT_LEFT_ANKLE"+QString::number(num)].x(),1-m_joints["JOINT_LEFT_ANKLE"+QString::number(num)].y(),0);
        glEnd();


}
void glView::setJointsMap(QMap <QString, QVector3D> joints_map)
{
    m_joints = joints_map;
    number_skeleton = std::round(m_joints.size()/number_joints);
    repaint();
}



