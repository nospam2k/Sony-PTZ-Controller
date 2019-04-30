#include "appsettings.h"
#include <QStandardPaths>
AppSettings::AppSettings()
{
    QString writablePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config";
    cameraIpList = new QSettings(writablePath + APP_CAMERAIP_LIST , QSettings::IniFormat);
    cameraList = new QSettings(writablePath + APP_CAMERA_LIST , QSettings::IniFormat);
    presetList = new QSettings(writablePath + APP_PRESET_LIST , QSettings::IniFormat);
    presetNameList = new QSettings(writablePath + APP_PRESETNAME_LIST , QSettings::IniFormat);
}
AppSettings::~AppSettings()
{
    delete cameraIpList;
    delete cameraList;
    delete presetList;
    delete presetNameList;
}
void AppSettings::saveCamera(SonyCam *cam)
{
    if(cam == nullptr)
        return;
    //save camera ip
    cameraIpList->setValue(cam->getCameraIp() , cam->getCameraName());
    cameraList->setValue(cam->getCameraIp() + "_port" , DEFAULT_TARGET_PORTNUM);
    cameraList->setValue(cam->getCameraIp() + "_callpresetspeed" , cam->getCallPresetSpeed());
    cameraList->setValue(cam->getCameraIp() + "_waittime" , cam->getWaitTime());

    //remove presets
    presetList->remove(cam->getCameraIp() + "_presets");
    presetList->beginWriteArray(cam->getCameraIp() + "_presets");
    for(int i = 0 ; i < cam->getPresetList().length() ; i ++)
    {
        presetList->setArrayIndex(i);
        presetList->setValue("preset_num" , cam->getPresetList().at(i).presetNum);
//        presetList->setValue("preset_name" , cam->getPresetList().at(i).presetName);
    }
    presetList->endArray();
}
void AppSettings::removeCamera(SonyCam *cam)
{
    if(cam == nullptr)
        return;
    cameraIpList->remove(cam->getCameraIp());
    cameraList->remove(cam->getCameraIp() + "_port");
    cameraList->remove(cam->getCameraIp() + "_callpresetspeed");
    cameraList->remove(cam->getCameraIp() + "_waittime");

    //remove presets
    presetList->remove(cam->getCameraIp() + "_presets");
}
//remove all data from the file when you change something
//and then save again


QList<QString> AppSettings::getCameraIpList()
{
    return cameraIpList->allKeys();
}
int AppSettings::getCameraCount()
{
    return getCameraIpList().length();
}
SonyCam *AppSettings::getCamera(int index)
{
    if(index < getCameraCount())
    {
        QString ip = getCameraIpList().at(index);
        QString camName = cameraIpList->value(ip , "Sony PTZ Camera").toString();
        int port = cameraList->value(ip + "_port" , DEFAULT_TARGET_PORTNUM).toInt();
        int waitTime = cameraList->value(ip + "waittime" , 10).toInt();
        int callPresetspeed = cameraList->value(ip + "_callpresetspeed" , 1).toInt();

        SonyCam *cam = new SonyCam(ip , DEFAULT_TARGET_PORTNUM , camName);
        cam->setWaitTime(waitTime);
        cam->setCameraPort(port);
        cam->setCallPresetSpeed(callPresetspeed);

        int len = presetList->beginReadArray(ip + "_presets");
        for(int i = 0 ; i < len ; i ++)
        {
            presetList->setArrayIndex(i);
            PRESET preset;
            preset.presetNum = presetList->value("preset_num" , 0).toInt();
            preset.presetName = getPresetName(cam->getCameraIp() , preset.presetNum);
            cam->addPreset(preset);
        }
        presetList->endArray();
        return cam;
    }
    return nullptr;
}
void AppSettings::setPresetName(QString ip, int presetNum, QString presetName)
{
    presetNameList->setValue(ip + "_" + QString::number(presetNum) , presetName);
}
QString AppSettings::getPresetName(QString ip, int presetNum)
{
    return presetNameList->value(ip + "_" + QString::number(presetNum) , "PRESET " + QString::number(presetNum)).toString();
}
void AppSettings::addNewPreset(QString ip, int presetNum)
{
    int len = presetList->beginReadArray(ip + "_presets");
    presetList->endArray();
    presetList->beginWriteArray(ip + "_presets");
    presetList->setArrayIndex(len);
    presetList->setValue("preset_num" , presetNum);
    presetList->endArray();
}
void AppSettings::editPreset(QString ip, int presetIndex, int presetNum)
{
    presetList->beginWriteArray(ip + "_presets");
    presetList->setArrayIndex(presetIndex);
    presetList->setValue("preset_num" , presetNum);
    presetList->endArray();
}
