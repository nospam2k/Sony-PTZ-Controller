#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>
#include "sonycam.h"
#define APP_CAMERAIP_LIST "./cameraips.ini"
#define APP_CAMERA_LIST "./cameras.ini"
#define APP_PRESET_LIST "./presets.ini"
#define APP_PRESETNAME_LIST "./presetname.ini"
class AppSettings : public QObject
{
    Q_OBJECT
public:
    AppSettings();
    ~AppSettings();
    void saveCamera(SonyCam *cam);
    void removeCamera(SonyCam *cam);
    int getCameraCount();
    SonyCam* getCamera(int index);
    void setPresetName(QString ip, int presetNum, QString presetName);
    void addNewPreset(QString ip , int presetNum);
    void editPreset(QString ip, int presetIndex, int presetNum);
    QString getPresetName(QString ip, int presetNum);

    void setCallPresetSpeed(SonyCam* cam);
    void setWaitTime(SonyCam* cam);
private:
    QSettings *cameraIpList;
    QSettings *cameraList;
    QSettings *presetList;
    QSettings *presetNameList;

    QList<QString> getCameraIpList();



};

#endif // APPSETTINGS_H
