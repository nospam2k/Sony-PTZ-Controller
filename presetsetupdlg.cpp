#include "presetsetupdlg.h"
#include "ui_presetsetupdlg.h"
#include "sonyptzapp.h"
#include "sonycam.h"
#include "sonycameramanager.h"
#include <string.h>
PresetSetupDlg::PresetSetupDlg(QWidget *parent , int presetIndex) :
    QDialog(parent),
    ui(new Ui::PresetSetupDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    this->presetIndex = presetIndex;


    initUi();
}

PresetSetupDlg::~PresetSetupDlg()
{
    delete ui;
}
void PresetSetupDlg::initUi()
{
    connect(ui->moveUpBtn , SIGNAL(pressed()) , this , SLOT(moveUp()));
    connect(ui->moveUpBtn , SIGNAL(released()) , this , SLOT(stopMoving()));
    connect(ui->moveDownBtn , SIGNAL(pressed()) , this , SLOT(moveDown()));
    connect(ui->moveDownBtn , SIGNAL(released()) , this , SLOT(stopMoving()));
    connect(ui->moveLeftBtn , SIGNAL(pressed()) , this , SLOT(moveLeft()));
    connect(ui->moveLeftBtn , SIGNAL(released()) , this , SLOT(stopMoving()));
    connect(ui->moveRightBtn , SIGNAL(pressed()) , this , SLOT(moveRight()));
    connect(ui->moveRightBtn , SIGNAL(released()) , this , SLOT(stopMoving()));
    connect(ui->moveHomeBtn , SIGNAL(clicked()) , this , SLOT(moveHome()));
    connect(ui->zoomInBtn , SIGNAL(pressed()) , this , SLOT(zoomInOnce()));
    connect(ui->zoomOutBtn , SIGNAL(pressed()) , this , SLOT(zoomOutOnce()));
    connect(ui->zoomInBtn , SIGNAL(released()) , this , SLOT(stopZooming()));
    connect(ui->zoomOutBtn , SIGNAL(released()) , this , SLOT(stopZooming()));
    connect(ui->focusInBtn , SIGNAL(pressed()) , this , SLOT(focusInOnce()));
    connect(ui->focusInBtn , SIGNAL(released()) , this , SLOT(stopFocusing()));
    connect(ui->focusOutBtn , SIGNAL(pressed()) , this , SLOT(focusOutOnce()));
    connect(ui->focusOutBtn , SIGNAL(released()) , this , SLOT(stopFocusing()));
    connect(ui->autoFocusRadio , SIGNAL(clicked()) , this , SLOT(changeFocusMode()));
    connect(ui->manualFocusRadio , SIGNAL(clicked()) , this , SLOT(changeFocusMode()));
    connect(ui->setPresetBtn , SIGNAL(clicked()) , this , SLOT(setPreset()));
    connect(ui->recallPreset , SIGNAL(clicked()) , this , SLOT(callPreset()));

    connect(ui->addPresetBtn , SIGNAL(clicked()) , this , SLOT(savePresetSetup()));
    connect(ui->cancelPresetBtn , SIGNAL(clicked()) , this , SLOT(cancelPresetSetup()));

    for(int i = 0 ; i < 64 ; i ++)
    {
        ui->presetCombo->addItem(QString::number(i + 1));
        ui->presetCombo1->addItem(QString::number(i + 1));
        ui->presetCombo2->addItem(QString::number(i + 1));
    }
    for(int i = 0 ; i < 18 ; i ++)
    {
        ui->panSpeedCombo->addItem(QString::number(i + 1));
        ui->tiltSpeedCombo->addItem(QString::number(i + 1));
    }
    for(int i = 0 ; i < 8 ; i ++)
    {
        ui->zoomSpeedCombo->addItem(QString::number(i + 1));
        ui->focusSpeedCombo->addItem(QString::number(i + 1));
    }

    if(presetIndex != -1)
        ui->presetCombo->setCurrentIndex(App()->getCameraManager()->getCurCam()->getPresetList().at(presetIndex).presetNum);
}
void PresetSetupDlg::moveUp()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->moveOneUp(ui->tiltSpeedCombo->currentIndex() + 1);

}
void PresetSetupDlg::moveDown()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->moveOneDown(ui->tiltSpeedCombo->currentIndex() + 1);
}
void PresetSetupDlg::moveLeft()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->moveOneLeft(ui->panSpeedCombo->currentIndex() + 1);
}
void PresetSetupDlg::moveRight()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->moveOneRight(ui->panSpeedCombo->currentIndex() + 1);
}
void PresetSetupDlg::moveHome()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->moveHome();
}
void PresetSetupDlg::stopMoving()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->stopMoving();
}
void PresetSetupDlg::zoomInOnce()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->zoomOneIn(ui->zoomSpeedCombo->currentIndex());
}
void PresetSetupDlg::zoomOutOnce()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->zoomOneOut(ui->zoomSpeedCombo->currentIndex());
}
void PresetSetupDlg::stopZooming()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->stopZooming();
}
void PresetSetupDlg::focusInOnce()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->focusOneIn(ui->focusSpeedCombo->currentIndex());
}
void PresetSetupDlg::focusOutOnce()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->focusOneOut(ui->focusSpeedCombo->currentIndex());
}
void PresetSetupDlg::stopFocusing()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->stopFocusing();
}
void PresetSetupDlg::changeFocusMode()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->setFocusMode(ui->autoFocusRadio->isChecked());
}
void PresetSetupDlg::setPreset()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    //should save preset name in file if there is name
    if(ui->presetNameEdit->text().length() != 0)
        App()->getAppSettings()->setPresetName(cam->getCameraIp() , ui->presetCombo1->currentIndex() , ui->presetNameEdit->text());
    cam->setPreset(ui->presetCombo1->currentIndex());
}
void PresetSetupDlg::callPreset()
{
    SonyCam* cam = App()->getCameraManager()->getCurCam();
    cam->callPreset(ui->presetCombo2->currentIndex());
}
void PresetSetupDlg::savePresetSetup()
{
    SonyCam *cam = App()->getCameraManager()->getCurCam();
    //store presets in curcam
    if(presetIndex == -1)//create
    {
        PRESET preset;
        preset.presetNum = ui->presetCombo->currentIndex();
        //should change to preset name from file

        App()->getAppSettings()->addNewPreset(cam->getCameraIp() , preset.presetNum);
        preset.presetName = App()->getAppSettings()->getPresetName(cam->getCameraIp() , preset.presetNum);

        //add new preset
        cam->addPreset(preset);
        //set the preset call speed
        cam->setPresetSpeed(preset.presetNum , App()->getCameraManager()->getCurCam()->getCallPresetSpeed());
    }
    else//edit
    {
        PRESET preset;
        preset.presetNum = ui->presetCombo->currentIndex();
        preset.presetName = App()->getAppSettings()->getPresetName(cam->getCameraIp() , preset.presetNum);
        //change preset info
        cam->replacePreset(preset , presetIndex);
        //should change to preset name from file
        App()->getAppSettings()->savePresets(cam);
        //set the preset call speed
        cam->setPresetSpeed(preset.presetNum , App()->getCameraManager()->getCurCam()->getCallPresetSpeed());
    }
    accept();
}
void PresetSetupDlg::cancelPresetSetup()
{
    close();
}
