#ifndef SONYCAM_H
#define SONYCAM_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>
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
    SonyCam(QString ip = QString("0.0.0.0"), unsigned int portNum = DEFAULT_TARGET_PORTNUM , QString cameraName = "Camera");
    ~SonyCam();

    bool isConnected;


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
    void focusOneIn(unsigned int focusSpeed);
    void focusOneOut(unsigned int focusSpeed);
    void setFocusMode(bool autoFocus);
    void stopFocusing();
    void stopMoving();
    void stopZooming();

    void setCameraIp(QString ip);
    void setCameraName(QString name);
    void setCameraPort(unsigned int port);

    QString getCameraIp();
    QString getCameraName();

    void startLooping();
    void stopLooping();

    void setCallPresetSpeed(unsigned int speed);
    void setWaiteTime(unsigned int seconds);


private:

    QString cameraIp;
    unsigned int portNum;
    QString cameraName;
    QUdpSocket *connector;
    unsigned int commandIndex;//used to build command header
    char toSendPacket[24];//to send packet max 24 byte

    QList<int> presetLoop;
    int callPresetSpeed = 1;//1 ~ 18
    int waiteTime = 10;//unit is second
    QTimer* timer;
    int counter;

    void loadPresets();
    void addPreset(int presetNum);
    void removePreset(int presetIndex);

    void initUdpConnector();
    void initTimer();
    void createMessageHeader(const unsigned int cmdHeader, const unsigned int payloadLen);
    void sendPacket(unsigned int len);

private slots:
    //socket signal processing functions
    void onClientConnected();
    void onClientDisconnected();
    void onError();
    void onTimerOneSec();
signals:
    void cameraNameChange();

};

#endif // SONYCAM_H
