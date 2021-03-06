#include "sonyptzapp.h"
#include <QMessageBox>
SonyPTZApp::SonyPTZApp(int argc, char *argv[]):QApplication(argc, argv)
{
    //create writable location
    //needed for saving logs...
    if(!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).exists())
    {
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    settings = new AppSettings();
    //load cameras
    cameraManager = new SonyCameraManager(this);
    mainWin = new MainWindow;
    cameraManager->loadCameras();
    showUi();
    sysTrayInit();

}
SonyPTZApp::~SonyPTZApp()
{
    //delete ui
    delete mainWin;

    //delete cameramanager
    delete cameraManager;

    //delete settings
    delete settings;
}
AppSettings *SonyPTZApp::getAppSettings()
{
    return settings;
}
SonyCameraManager *SonyPTZApp::getCameraManager()
{
    return cameraManager;
}
MainWindow *SonyPTZApp::getMainWindow()
{
    return mainWin;
}
void SonyPTZApp::sysTrayInit()
{
    appTrayIcon = new QSystemTrayIcon(QIcon(":/assets/sonyptz.png"));
    actionExit = new QAction("Exit" , appTrayIcon);
    connect(actionExit , &QAction::triggered , [this]{
       this->quit();
    });
    actionShowHide = new QAction("Show/Hide" , appTrayIcon);
    connect(actionShowHide , SIGNAL(triggered()) , this , SLOT(toggleUi()));
    trayMenu = new QMenu();
    trayMenu->addAction(actionShowHide);
    trayMenu->addAction(actionExit);
    appTrayIcon->setContextMenu(trayMenu);
    appTrayIcon->show();

}
void SonyPTZApp::quitApp()
{
    if(this->mainWin->isVisible())
        hideUi();
    this->quit();
}
void SonyPTZApp::toggleUi()
{
    if(mainWin->isVisible())
        hideUi();
    else
        showUi();
}
void SonyPTZApp::showUi()
{
    mainWin->show();
}
void SonyPTZApp::hideUi()
{
    mainWin->close();
}
void SonyPTZApp::showMessage(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}
