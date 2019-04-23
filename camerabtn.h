#ifndef CAMERABTN_H
#define CAMERABTN_H

#include <QPushButton>
#include "sonycam.h"
class CameraBtn : public QPushButton
{
    Q_OBJECT
public:
    CameraBtn(QWidget *parent, SonyCam *cam);
};

#endif // CAMERABTN_H
