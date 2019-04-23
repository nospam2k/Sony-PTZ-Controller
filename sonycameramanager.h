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
private:
    QList<SonyCam *> cameraList;
    QUdpSocket *receiver;
    void initUdpSocket();
    void processReceivedData(QDataStream *datagram);

signals:
    void dataReceived(QString command, int commandNum);
private slots:
    void readyToRead();

public slots:
};

#endif // SONYCAMERAMANAGER_H
