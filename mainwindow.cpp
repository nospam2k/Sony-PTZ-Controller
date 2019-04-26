#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <QGridLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "camerasetupdlg.h"
#include "presetsetupdlg.h"
#include "sonyptzapp.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initUi()
{
    connect(ui->addCamBtn , SIGNAL(clicked(bool)) , this , SLOT(addCamera()));
    connect(ui->editCamBtn , SIGNAL(clicked(bool)) , this , SLOT(editCameraIp()));
    connect(ui->removeCamBtn , SIGNAL(clicked(bool)) , this , SLOT(removeCamera()));
    connect(ui->connectCamBtn , SIGNAL(clicked(bool)) , this , SLOT(connectToCamera()));
    connect(ui->clearListBtn , SIGNAL(clicked(bool)) , this , SLOT(clearRxTxList()));
    connect(ui->saveListBtn , SIGNAL(clicked(bool)) , this , SLOT(saveAsFile()));
    connect(ui->removePresetBtn , SIGNAL(clicked(bool)) , this , SLOT(removePreset()));
    connect(ui->addPresetBtn , SIGNAL(clicked(bool)) , this , SLOT(addPreset()));
    connect(ui->startLoopingBtn , SIGNAL(clicked(bool)) , this , SLOT(startLooping()));
    connect(ui->stopLoopingBtn , SIGNAL(clicked(bool)) , this , SLOT(stopLooping()));
    connect(ui->waitTimeDial , SIGNAL(valueChanged(int)) , this , SLOT(waitTimeChanged(int)));
    connect(ui->moveSpeedDial , SIGNAL(valueChanged(int)) , this , SLOT(speedChanged(int)));

    connect(App()->getCameraManager() , SIGNAL(curCamChanged()) , this , SLOT(curCamChanged()));
    connect(App()->getCameraManager() , SIGNAL(curCamParamChanged()) , this , SLOT(curCamParamChanged()));

    ui->waitTimeDial->setValue(1);
    ui->moveSpeedDial->setValue(1);
}

void MainWindow::addCamera()
{
    CameraSetupDlg camSetupDlg;
    if(camSetupDlg.exec() == QDialog::Accepted)
    {
        //create a button
        CameraBtn *newCamBtn = new CameraBtn(ui->cameraBtnWrapper , App()->getCameraManager()->getCam(App()->getCameraManager()->getCamCount() - 1));
        auto cameraClicked = [this, newCamBtn]{
            this->cameraBtnClicked(newCamBtn);
        };
        connect(newCamBtn , &QPushButton::clicked , cameraClicked);
        buttonList.append(newCamBtn);
        //relayout
//        for(int i = 0 ; i < buttonList.length() - 1 ; i ++)
//            ((QGridLayout*)(ui->cameraBtnWrapper->layout()))->removeWidget(buttonList.at(i));
        for(int i = 0 ; i < buttonList.length() ; i ++)
            ((QGridLayout*)(ui->cameraBtnWrapper->layout()))->addWidget(buttonList.at(i) , i / 5 , i % 5);
    }
}
void MainWindow::editCameraIp()
{
//    CameraSetupDlg camSetupDlg();
    //TODO
    SonyCam *curCam = App()->getCameraManager()->getCurCam();
    if(curCam == nullptr)
        return;
    CameraSetupDlg camSetupDlg(this , curCam);
    camSetupDlg.exec();
}
void MainWindow::removeCamera()
{
    //get index from camera manager
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    if(cam == nullptr)
        return;
    int index = App()->getCameraManager()->getCamIndex(App()->getCameraManager()->getCurCam());
    //remove camera
    App()->getCameraManager()->removeCamera(cam);
    //change cur cam
    App()->getCameraManager()->setCurCam(nullptr);

    for(int i = 0 ; i < buttonList.length() ; i ++)
        ((QGridLayout*)(ui->cameraBtnWrapper->layout()))->removeWidget(buttonList.at(i));
    //remove button
    CameraBtn* btn = buttonList.at(index);
    buttonList.removeAt(index);
    delete btn;
    for(int i = 0 ; i < buttonList.length() ; i ++)
        ((QGridLayout*)(ui->cameraBtnWrapper->layout()))->addWidget(buttonList.at(i) , i / 5 , i % 5);


}
void MainWindow::connectToCamera()
{
    //connect & disconnect camera
    SonyCam *curCam = App()->getCameraManager()->getCurCam();
    if(curCam != nullptr)
    {
        if(curCam->isConnected)
            curCam->disconnectFromCamera();
        else
            curCam->connectToCamera();
    }
}
void MainWindow::clearRxTxList()
{
    ui->txRxList->clear();
}
void MainWindow::saveAsFile()
{
    QString logs = ui->txRxList->toPlainText();
    QDateTime current = QDateTime::currentDateTime();
    QFile *file = new QFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + current.toString("yy-MM-dd hh-mm-ss") + ".log");
    if(file->open(QFile::Append))
    {
        file->write(logs.toUtf8());
        file->flush();
        file->close();
    }
    file->deleteLater();
}

void MainWindow::removePreset()
{

}
void MainWindow::addPreset()
{
    PresetSetupDlg presetSetupDlg;
    if(presetSetupDlg.exec())
    {

    }
    else
    {

    }
}

void MainWindow::startLooping()
{

}
void MainWindow::stopLooping()
{

}

void MainWindow::waitTimeChanged(int curWaitTime)
{
    ui->waitTimeLcd->display(curWaitTime);
    //change wait time in sonycameramanager
    //TODO
}
void MainWindow::speedChanged(int curSpeed)
{
    ui->moveSpeedLcd->display(curSpeed);
    //change speed of camera
    //TODO
}
void MainWindow::cameraBtnClicked(CameraBtn* btn)
{
    //change the background color of all buttons as default
    for(int i = 0 ; i < buttonList.length() ; i ++)
        buttonList.at(i)->changeBackgroundAsDefault();
    btn->changeBackgroundAsPressed();
    App()->getCameraManager()->setCurCam(buttonList.indexOf(btn));

}
void MainWindow::curCamChanged()
{
    SonyCam *curCam = App()->getCameraManager()->getCurCam();
    if(curCam != nullptr)
    {
        //ip address
        ui->ipLcd->display(curCam->getCameraIp());
        //connection status
        if(curCam->isConnected)
        {
            showCameraStatus(0);
        }
        else
        {
            showCameraStatus(1);
        }
        //fill presets list
        //TODO
        //change the waittime
        //TODO
        //change the speed
        //TODO
    }
    else
    {
        //change as default
        ui->ipLcd->display(0);
        showCameraStatus(2);
        ui->presetList->clear();
        ui->waitTimeDial->setValue(1);
        ui->moveSpeedDial->setValue(1);

    }
}
void MainWindow::showCameraStatus(int status)
{
    switch(status)
    {
    case 0:
        ui->connectCamBtn->setText("Disconnect");
        ui->connectCamBtn->setIcon(QIcon(":/assets/toggle-on.png"));
        break;
    case 1:
        ui->connectCamBtn->setText("Connect");
        ui->connectCamBtn->setIcon(QIcon(":/assets/toggle-off.png"));
        break;
    case 2:
        ui->connectCamBtn->setText("Connect");
        ui->connectCamBtn->setIcon(QIcon(":/assets/toggle-mid.png"));
        break;
    }
}
void MainWindow::curCamParamChanged()
{

}
