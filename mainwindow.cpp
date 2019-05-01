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
    for(int i = 0 ; i < buttonList.length() ; i ++)
    {
        CameraBtn *btn = buttonList.at(0);
        buttonList.removeAt(0);;
        delete btn;
    }
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
    connect(ui->addPresetBtn , SIGNAL(clicked(bool)) , this , SLOT(addPreset()));
    connect(ui->startLoopingBtn , SIGNAL(clicked(bool)) , this , SLOT(startLooping()));
    connect(ui->stopLoopingBtn , SIGNAL(clicked(bool)) , this , SLOT(stopLooping()));
    connect(ui->waitTimeDial , SIGNAL(valueChanged(int)) , this , SLOT(waitTimeChanged(int)));
    connect(ui->moveSpeedDial , SIGNAL(valueChanged(int)) , this , SLOT(speedChanged(int)));
    connect(ui->movementSpeedOKBtn , SIGNAL(clicked(bool)) , this , SLOT(onMoveSpeedEdited()));
    connect(ui->waitTimeOKBtn , SIGNAL(clicked(bool)) , this , SLOT(onWaitTimeEdited()));

    connect(App()->getCameraManager() , SIGNAL(curCamChanged()) , this , SLOT(curCamChanged()));

    connect(App()->getCameraManager() , SIGNAL(curCamError()) , this , SLOT(curCamError()));
    connect(App()->getCameraManager() , SIGNAL(curCamConnected()) , this , SLOT(curCamConnected()));
    connect(App()->getCameraManager() , SIGNAL(curCamDisconnected()) , this , SLOT(curCamDisconnected()));
    connect(App()->getCameraManager() , SIGNAL(curCamIpChanged()) , this , SLOT(curCamIpChanged()));
    connect(App()->getCameraManager() , SIGNAL(reportData(QString)) , this , SLOT(onReportReady(QString)));
    connect(App()->getCameraManager() , SIGNAL(cameraAdded()) , this , SLOT(onCameraAdded()));
    connect(App()->getCameraManager() , SIGNAL(curCamLoopingStarted()) , this , SLOT(onCurCamLoopingStarted()));
    connect(App()->getCameraManager() , SIGNAL(curCamLoopingStopped()) , this , SLOT(onCurCamLoopingStopped()));

    ui->waitTimeLcd->display(ui->waitTimeDial->value());
    ui->moveSpeedLcd->display(ui->moveSpeedDial->value());
    ui->waitTimeEdit->setText(QString::number(ui->waitTimeDial->value()));
    ui->movementSpeedEdit->setText(QString::number(ui->moveSpeedDial->value()));
}

void MainWindow::addCamera()
{
    CameraSetupDlg camSetupDlg;
    camSetupDlg.exec();
}
void MainWindow::loadCameraBtnList()
{

}
void MainWindow::onCameraAdded()
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
void MainWindow::editCameraIp()
{
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

    //remove from file
    App()->getAppSettings()->removeCamera(cam);

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
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + current.toString("yy-MM-dd hh-mm-ss") + ".log";
    QFile *file = new QFile(filePath);
    if(file->open(QFile::Append))
    {
        file->write(logs.toUtf8());
        file->flush();
        file->close();
    }
    file->deleteLater();
    App()->showMessage("Saved to " + filePath);
}

void MainWindow::addPreset()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    if(cam == nullptr)
        return;
    PresetSetupDlg presetSetupDlg;
    if(presetSetupDlg.exec() == QDialog::Accepted)
    {
        PRESET preset;
        preset = cam->getPresetList().at(cam->getPresetList().length() - 1);
        addPresetWidget(cam->getPresetList().length() - 1);
    }

}
void MainWindow::loadPresetList()
{
    SonyCam *curCam = App()->getCameraManager()->getCurCam();
    if(curCam == nullptr)
        return;

}
void MainWindow::addPresetWidget(int presetIndex)
{
    QListWidgetItem *item = new QListWidgetItem(ui->presetList);
    ui->presetList->addItem(item);
    item->setSizeHint(QSize(ui->presetList->size().width() - 4 , 42));
    ui->presetList->setItemWidget(item , getListItem(presetIndex));
}
ListItem* MainWindow::getListItem(int presetIndex)
{
    return new ListItem(ui->presetList , presetIndex);
}
void MainWindow::startLooping()
{
    App()->getCameraManager()->getCurCam()->startLooping();
}
void MainWindow::stopLooping()
{
    App()->getCameraManager()->getCurCam()->stopLooping();
}

void MainWindow::waitTimeChanged(int curWaitTime)
{
    ui->waitTimeLcd->display(curWaitTime);
    ui->waitTimeEdit->setText(QString::number(curWaitTime));
    //change wait time of camera
    SonyCam* curCam = App()->getCameraManager()->getCurCam();
    if(curCam == nullptr)
        return;
    curCam->setWaitTime(curWaitTime);
    App()->getAppSettings()->setWaitTime(curCam);
}
void MainWindow::speedChanged(int curSpeed)
{
    ui->moveSpeedLcd->display(curSpeed);
    ui->movementSpeedEdit->setText(QString::number(curSpeed));
    SonyCam* curCam = App()->getCameraManager()->getCurCam();
    if(curCam == nullptr)
        return;
    curCam->setCallPresetSpeed(curSpeed);
    App()->getAppSettings()->setCallPresetSpeed(curCam);
}
void MainWindow::onWaitTimeEdited()
{
    int waitTime = ui->waitTimeEdit->text().toInt();
    qDebug()<<waitTime;
    if(waitTime < 1)
        waitTime = 1;
    if(waitTime > 99)
        waitTime = 99;
    waitTimeChanged(waitTime);

}
void MainWindow::onMoveSpeedEdited()
{
    int moveSpeed = ui->movementSpeedEdit->text().toInt();
    qDebug()<<moveSpeed;
    if(moveSpeed < 1)
        moveSpeed = 1;
    if(moveSpeed > 18)
        moveSpeed = 18;
    speedChanged(moveSpeed);
}
void MainWindow::cameraBtnClicked(CameraBtn* btn)
{
    //change the background color of all buttons as default
    for(int i = 0 ; i < buttonList.length() ; i ++)
        buttonList.at(i)->changeBackgroundAsDefault();
    btn->changeBackgroundAsPressed();
    App()->getCameraManager()->setCurCam(buttonList.indexOf(btn));
    ui->waitTimeDial->setValue(App()->getCameraManager()->getCurCam()->getWaitTime());
    ui->moveSpeedDial->setValue(App()->getCameraManager()->getCurCam()->getCallPresetSpeed());


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
        for(int i = 0 ; i < curCam->getPresetList().length() ; i ++)
        {

            addPresetWidget(i);
        }
        //change the waittime
        ui->waitTimeDial->setValue(curCam->getWaitTime());
        //change the speed
        ui->moveSpeedDial->setValue(curCam->getCallPresetSpeed());
        showLoopingStatus(curCam->isLooping());
    }
    else
    {
        //change as default
        ui->ipLcd->display(0);
        showCameraStatus(2);
        ui->presetList->clear();
        ui->waitTimeDial->setValue(1);
        ui->moveSpeedDial->setValue(1);
        showLoopingStatus(false);
    }
}
void MainWindow::onReportReady(QString report)
{
    ui->txRxList->append(report);
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

void MainWindow::curCamError()
{
    App()->showMessage(App()->getCameraManager()->getCurCam()->getErrorMessage());
}
void MainWindow::curCamConnected()
{
    showCameraStatus(0);
}
void MainWindow::curCamDisconnected()
{
    showCameraStatus(1);
}
void MainWindow::curCamIpChanged()
{
    ui->ipLcd->display(App()->getCameraManager()->getCurCam()->getCameraIp());
}
void MainWindow::onCurCamLoopingStarted()
{
    showLoopingStatus(true);
}
void MainWindow::onCurCamLoopingStopped()
{
    showLoopingStatus(false);
}
void MainWindow::showLoopingStatus(bool looping)
{
    if(looping)
        ui->startLoopingBtn->setIcon(QIcon(":/assets/toggle-on.png"));
    else
        ui->startLoopingBtn->setIcon(QIcon(":/assets/toggle-mid.png"));
}
