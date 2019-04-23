#include "sonyptzapp.h"

SonyPTZApp::SonyPTZApp(int argc, char *argv[]):QApplication(argc, argv)
{
    settings = new AppSettings();
    //load cameras
    //TODO
    mainWin = new MainWindow;
    mainWin->show();
}
SonyPTZApp::~SonyPTZApp()
{
    //stop camera working and delete all camera intances
    //TODO
    //delete ui
    delete mainWin;
    //delete settings
    delete settings;
}
