#ifndef SONYPTZAPP_H
#define SONYPTZAPP_H

#include <QObject>
#include <QApplication>
#include "sonycam.h"
#include "appsettings.h"
#include "mainwindow.h"
#include "sonycameramanager.h"
class SonyPTZApp : public QApplication
{
    Q_OBJECT
public:
    SonyPTZApp(int argc, char *argv[]);
    ~SonyPTZApp();
private:
    AppSettings *settings;
    SonyCameraManager *cameraManager;
    MainWindow *mainWin;
};
inline SonyPTZApp *App() {return static_cast<SonyPTZApp*>(qApp);}
#endif // SONYPTZAPP_H
