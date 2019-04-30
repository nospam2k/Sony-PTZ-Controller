#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "camerabtn.h"
#include "listitem.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<CameraBtn*> buttonList;
    void initUi();
    void cameraBtnClicked(CameraBtn *btn);

    void showCameraStatus(int status);//0 connected, 1 disconnected, 2 default

    void loadCameraBtnList();
    void loadPresetList();

    void addPresetWidget(int presetIndex);
    ListItem* getListItem(int presetIndex);
private slots:
    void addCamera();
    void editCameraIp();
    void removeCamera();
    void connectToCamera();
    void clearRxTxList();
    void saveAsFile();

    void addPreset();

    void startLooping();
    void stopLooping();

    void waitTimeChanged(int curWaitTime);
    void speedChanged(int curSpeed);

    void curCamChanged();
    void curCamPresetsChanged();
    void curCamIpChanged();
    void curCamError();
    void curCamConnected();
    void curCamDisconnected();

    void onReportReady(QString report);

    void onCameraAdded();





};

#endif // MAINWINDOW_H
