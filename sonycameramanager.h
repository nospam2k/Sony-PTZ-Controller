#ifndef SONYCAMERAMANAGER_H
#define SONYCAMERAMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include "sonycam.h"
#define DEFAULT_RECEIVE_PORTNUM 52381
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
private:
    QList<SonyCam *> cameraList;
    QUdpSocket *receiver;
    SonyCam *curCam;
    void initUdpSocket();
    void processReceivedData(QNetworkDatagram datagram);

signals:
    void dataReceived(QString command, int commandNum);
    void curCamChanged();
    void curCamParamChanged();
private slots:
    void readyToRead();

public slots:
};

#endif // SONYCAMERAMANAGER_H
