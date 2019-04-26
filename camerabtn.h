#ifndef CAMERABTN_H
#define CAMERABTN_H

#include <QPushButton>
#include "sonycam.h"
class CameraBtn : public QPushButton
{
    Q_OBJECT
public:
    CameraBtn(QWidget *parent, SonyCam *cam);
    SonyCam* getCam();

    void changeBackgroundAsDefault();
    void changeBackgroundAsPressed();
private:
    SonyCam *cam;
private slots:
    void cameraNameChanged();

};

#endif // CAMERABTN_H
