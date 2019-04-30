#include "camerabtn.h"

CameraBtn::CameraBtn(QWidget *parent, SonyCam *cam):QPushButton(parent)
{
    this->setText(cam->getCameraName());
    this->cam = cam;
    connect(cam, SIGNAL(cameraNameChanged()) , this , SLOT(cameraNameChanged()));
}
void CameraBtn::cameraNameChanged()
{
    this->setText(cam->getCameraName());
}
SonyCam* CameraBtn::getCam()
{
    return cam;
}

void CameraBtn::changeBackgroundAsDefault()
{
    this->setStyleSheet("QPushButton{background : grey;}");
}
void CameraBtn::changeBackgroundAsPressed()
{
    this->setStyleSheet("QPushButton{background : #474747;}");
}
