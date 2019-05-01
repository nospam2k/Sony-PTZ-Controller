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
typedef struct PRESET{
    int presetNum;
    QString presetName;
}PRESET;
typedef struct COMMAND{
    char commandByte[24];
    int commandLength;
}COMMAND;
class SonyCam : public QObject
{
    Q_OBJECT
public:
    SonyCam(QString ip = QString("0.0.0.0"), unsigned int portNum = DEFAULT_TARGET_PORTNUM , QString cameraName = "Camera");
    ~SonyCam();

    bool isConnected;

    void connectToCamera();
    void disconnectFromCamera();


    void setPresetSpeed(unsigned int presetNum , unsigned int speed);
    void setPreset(unsigned int presetNum);
    void callPreset(unsigned int presetNum);
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
    void initMessaging();

    void setCameraIp(QString ip);
    void setCameraName(QString name);
    void setCameraPort(unsigned int port);

    QString getCameraIp();
    QString getCameraName();

    void startLooping();
    void stopLooping();

    void setCallPresetSpeed(unsigned int speed);
    void setWaitTime(unsigned int seconds);
    unsigned int getCallPresetSpeed();
    unsigned int getWaitTime();

    QString getErrorMessage();
    void addPreset(PRESET preset);
    void removePreset(int presetIndex);
    void replacePreset(PRESET preset, int presetIndex);
    QList<PRESET> getPresetList();

    QList<COMMAND *> commandQue;
    void onReceiveReply();
    void onReceiveACK();

    bool isLooping();


private:

    bool isWaitingReply;

    QString cameraIp;//tosave
    unsigned int portNum;//tosave
    QString cameraName;//tosave
    QUdpSocket *connector;
    unsigned int commandIndex;//used to build command header
    char toSendPacket[24];//to send packet max 24 byte

    QList<PRESET> presetLoop;//tosave
    int callPresetSpeed = 1;//1 ~ 18//tosave
    int waiteTime = 10;//unit is second//tosave
    QTimer* timer;
    int counter;
    int curPresetIndex;

    void initUdpConnector();
    void initTimer();
    void createMessageHeader(const unsigned int cmdHeader, const unsigned int payloadLen);

    void addInTheQue(unsigned int len);
    void sendPacket();



private slots:
    //socket signal processing functions
    void onClientConnected();
    void onClientDisconnected();
    void onError();
    void onTimerOneSec();

signals:
    void cameraNameChanged();
    void cameraConnected();
    void cameraDisconnected();
    void cameraError();
    void cameraIpChanged();

    void messageSent(int type , QString bytes , int comIndex , SonyCam* cam);

    void loopingStarted(SonyCam *cam);
    void loopingStopped(SonyCam *cam);
};

#endif // SONYCAM_H
