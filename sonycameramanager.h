#ifndef SONYCAMERAMANAGER_H
#define SONYCAMERAMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include "sonycam.h"
#define DEFAULT_RECEIVE_PORTNUM 52381
#define MAX_BUFFER_SIZE 32
class SonyCameraManager : public QObject
{
    Q_OBJECT
public:
    explicit SonyCameraManager(QObject *parent = nullptr);
    ~SonyCameraManager();
    void removeCamera(SonyCam *cam);
    void addCamera(SonyCam *cam);

    SonyCam* getCam(int index);
    int getCamCount();
    SonyCam* getCurCam();
    void setCurCam(int index);
    void setCurCam(SonyCam* cam);
    int getCamIndex(SonyCam *cam);
    SonyCam* getCam(QHostAddress ipAddr);
private:
    QList<SonyCam *> cameraList;
    QUdpSocket *receiver;
    SonyCam *curCam;
    void initUdpSocket();
    void processReceivedData(QNetworkDatagram datagram);

    void buildReportData(int type , QString bytes , int comIndex);
signals:
    void dataReceived(QString command, int commandNum);
    void curCamChanged();
    void curCamIpChanged();
    void curCamPresetsChanged();
    void curCamError();
    void curCamConnected();
    void curCamDisconnected();
    void reportData(QString data);

private slots:
    void readyToRead();
    void onMessageSent(int type , QString bytes , int comIndex , SonyCam* cam);


public slots:
};

#endif // SONYCAMERAMANAGER_H
