#ifndef SONYCAM_H
#define SONYCAM_H

#include <QObject>
#include <QtNetwork>
#define DEFAULT_TARGET_PORTNUM 52381
#define COMMAND_HEAD 0x0100
#define INQUERY_HEAD 0x0110
#define CONTROL_COMMAND_HEAD 0x0200
#define CONTROL_COMMAND_REPLY_HEAD 0x0201
#define REPLY_HEAD 0x0111
class SonyCam : public QObject
{
    Q_OBJECT
public:
    SonyCam(QString ip = QString("0.0.0.0"), unsigned int portNum = DEFAULT_TARGET_PORTNUM);
    ~SonyCam();

    void connectToCamera();
    void disconnectFromCamera();


    void goToPreset(unsigned int presetNum , unsigned int speed);
    void moveOneLeft(unsigned int panSpeed);
    void moveOneRight(unsigned int panSpeed);
    void moveOneUp(unsigned int tiltSpeed);
    void moveOneDown(unsigned int tiltSpeed);
    void moveHome();
    void zoomOneIn(unsigned int zoomSpeed);
    void zoomOneOut(unsigned int zoomSpeed);
    void stopMoving();
    void stopZooming();

    void setCameraIp(QString ip);
    void setCameraPort(unsigned int port);

    QString getCameraIp();


private:
    QString cameraIp;
    unsigned int portNum;
    QUdpSocket *connector;
    unsigned int commandIndex;

    char toSendPacket[24];
    void initUdpConnector();
    void createMessageHeader(const unsigned int cmdHeader, const unsigned int payloadLen);
    void sendPacket(unsigned int len);

private slots:
    //socket signal processing functions
    void onClientConnected();
    void onClientDisconnected();
    void onError();

};

#endif // SONYCAM_H
