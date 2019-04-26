#ifndef SONYPTZAPP_H
#define SONYPTZAPP_H

#include <QObject>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
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
    AppSettings *getAppSettings();
    SonyCameraManager *getCameraManager();
    MainWindow *getMainWindow();
    void showMessage(QString msg);

private:
    AppSettings *settings;
    SonyCameraManager *cameraManager;
    MainWindow *mainWin;

    void sysTrayInit();

    QSystemTrayIcon *appTrayIcon;
    QAction *actionShowHide;
    QAction *actionExit;
    QMenu *trayMenu;

    void quitApp();
    void showUi();
    void hideUi();
private slots:
    void toggleUi();


};
inline SonyPTZApp *App() {return static_cast<SonyPTZApp*>(qApp);}
#endif // SONYPTZAPP_H
