#include "camerabtn.h"

CameraBtn::CameraBtn(QWidget *parent, SonyCam *cam):QPushButton(parent)
{
    this->setText(cam->cameraName);
}
