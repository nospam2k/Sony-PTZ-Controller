#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QSettings>

#define APP_SETTING_LOCATION "./settings.ini"

class AppSettings : public QObject
{
    Q_OBJECT
public:
    AppSettings();
};

#endif // APPSETTINGS_H
