#include "sonycam.h"

SonyCam::SonyCam(QString ip, unsigned int portNum , QString cameraName)
{
    isConnected = false;
    this->cameraIp = ip;
    this->portNum = portNum;
    this->cameraName = cameraName;
    initUdpConnector();
    initTimer();
    //load presets list
    loadPresets();
}
SonyCam::~SonyCam()
{
    stopLooping();
    delete timer;
    if(connector->isOpen())
        connector->close();
    delete connector;

}
void SonyCam::initUdpConnector()
{
    connector = new QUdpSocket(this);
    connect(connector , SIGNAL(connected()) , this , SLOT(onClientConnected()));
    connect(connector , SIGNAL(disconnected()) , this , SLOT(onClientDisconnected()));
    connect(connector , SIGNAL(error(QAbstractSocket::SocketError)) , this , SLOT(onError()));
}
void SonyCam::initTimer()
{
    timer = new QTimer(this);
    connect(timer , SIGNAL(timeout()) , this , SLOT(onTimerOneSec()));
}
void SonyCam::onClientConnected()
{
    isConnected = true;
    //initiate
    commandIndex = 0;
    toSendPacket[8] = 0x01;
    sendPacket(1);
    //TODO
}
void SonyCam::onClientDisconnected()
{
    isConnected = false;
    //TODO
}
void SonyCam::onError()
{
    //TODO
}
void SonyCam::onTimerOneSec()
{
    //increase counter
    counter ++;
    if(counter >= waiteTime)
    {
        counter = 0;
        //call next preset
        //TODO
    }
}
void SonyCam::connectToCamera()
{
    connector->connectToHost(cameraIp , DEFAULT_TARGET_PORTNUM);
}
void SonyCam::disconnectFromCamera()
{
    connector->disconnectFromHost();
}
void SonyCam::setCameraIp(QString ip)
{
    this->cameraIp = ip;
    disconnectFromCamera();
}
void SonyCam::setCameraName(QString name)
{
    this->cameraName = name;
    emit cameraNameChange();
}
void SonyCam::setCameraPort(unsigned int port)
{
    this->portNum = port;
    disconnectFromCamera();
}
QString SonyCam::getCameraIp()
{
    return cameraIp;
}
QString SonyCam::getCameraName()
{
    return cameraName;
}
void SonyCam::goToPreset(unsigned int presetNum , unsigned int speed)
{
    //81 01 7E 01 0B pp qq FF
    createMessageHeader(COMMAND_HEAD , 8);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x7e;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = 0x0b;
    toSendPacket[13] = (char)(presetNum % 64);//max preset number 64
    toSendPacket[14] = (char)(speed % 18 + 1);//max speed 18
    toSendPacket[15] = 0xff;
    sendPacket(8);
}
void SonyCam::loadPresets()
{
    //load presets from the ini file
    //TODO
}
void SonyCam::addPreset(int presetNum)
{
    presetLoop.append(presetNum);
}
void SonyCam::removePreset(int presetIndex)
{
    presetLoop.removeAt(presetIndex);
}
void SonyCam::startLooping()
{
    //start timer
    timer->start(1000);//start looping every 1 sec
    counter = 0;
}
void SonyCam::stopLooping()
{
    //stop timer
    timer->stop();
    //send message
    stopMoving();
    stopZooming();
    stopFocusing();
}
void SonyCam::setCallPresetSpeed(unsigned int speed)
{
    this->callPresetSpeed = speed;
}
void SonyCam::setWaiteTime(unsigned int seconds)
{
    this->waiteTime = seconds;
}
void SonyCam::moveOneLeft(unsigned int panSpeed)
{
    //81 01 06 01 vv ww 01 03 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = (char)(panSpeed % 18 + 1);//max pan speed 18
    toSendPacket[13] = 5;//default tilt speed
    toSendPacket[14] = 0x01;
    toSendPacket[15] = 0x03;
    toSendPacket[16] = 0xff;
    sendPacket(9);
}
void SonyCam::moveOneRight(unsigned int panSpeed)
{
    //81 01 06 01 vv ww 02 03 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = (char)(panSpeed % 18 + 1);//max pan speed 18
    toSendPacket[13] = 5;//default tilt speed
    toSendPacket[14] = 0x02;
    toSendPacket[15] = 0x03;
    toSendPacket[16] = 0xff;
    sendPacket(9);
}
void SonyCam::moveOneUp(unsigned int tiltSpeed)
{
    //81 01 06 01 vv ww 03 01 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = 5;//default pan speed
    toSendPacket[13] = (char)(tiltSpeed % 18 + 1);//max tilt speed 18
    toSendPacket[14] = 0x03;
    toSendPacket[15] = 0x01;
    toSendPacket[16] = 0xff;
    sendPacket(9);
}
void SonyCam::moveOneDown(unsigned int tiltSpeed)
{
    //81 01 06 01 vv ww 03 02 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = 5;//default pan speed
    toSendPacket[13] = (char)(tiltSpeed % 18 + 1);//max tilt speed 18
    toSendPacket[14] = 0x03;
    toSendPacket[15] = 0x02;
    toSendPacket[16] = 0xff;
    sendPacket(9);
}
void SonyCam::moveHome()
{
    //81 01 06 04 FF
    createMessageHeader(COMMAND_HEAD , 5);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x04;
    toSendPacket[12] = 0xff;
    sendPacket(5);
}
void SonyCam::zoomOneIn(unsigned int zoomSpeed)
{
    zoomSpeed = zoomSpeed % 8;//max zoom speed 7
    //81 01 04 07 2p FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x07;
    toSendPacket[12] = 0x20 + zoomSpeed;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::zoomOneOut(unsigned int zoomSpeed)
{
    zoomSpeed = zoomSpeed % 8;//max zoom speed 7
    //81 01 04 07 3p FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x07;
    toSendPacket[12] = 0x30 + zoomSpeed;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::focusOneIn(unsigned int focusSpeed)
{
    focusSpeed = focusSpeed % 8;//max focus speed 7
    //81 01 04 08 3p FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x08;
    toSendPacket[12] = 0x30 + focusSpeed;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::focusOneOut(unsigned int focusSpeed)
{
    focusSpeed = focusSpeed % 8;//max focus speed 7
    //81 01 04 08 2p FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x08;
    toSendPacket[12] = 0x20 + focusSpeed;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::stopFocusing()
{
    //81 01 04 08 00 FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x80;
    toSendPacket[12] = 0x00;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::setFocusMode(bool autoFocus)
{
    //81 01 04 38 02 FF - auto
    //81 01 04 38 03 FF - manual
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x38;
    if(autoFocus)
        toSendPacket[12] = 0x02;
    else
        toSendPacket[12] = 0x03;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::stopMoving()
{
    //81 01 04 07 00 FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x07;
    toSendPacket[12] = 0x00;
    toSendPacket[13] = 0xff;
    sendPacket(6);
}
void SonyCam::stopZooming()
{
    //81 01 06 01 vv ww 03 03 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = 5;//default pan speed
    toSendPacket[13] = 5;//default tilt speed
    toSendPacket[14] = 0x03;
    toSendPacket[15] = 0x03;
    toSendPacket[16] = 0xff;
    sendPacket(9);
}
/**
 * create message 8 byte message header
 * @brief createMessageHeader
 * @param cmdHeader: 0x0100, 0x0110, 0x0111, 0x0200, 0x0201
 * @param payloadLen: 1 ~ 16
 */
void SonyCam::createMessageHeader(const unsigned int cmdHeader, const unsigned int payloadLen)
{
    memset(toSendPacket , 0 , 24);
    toSendPacket[0] = cmdHeader / 256;
    toSendPacket[1] = cmdHeader % 256;
    toSendPacket[2] = payloadLen / 256;
    toSendPacket[3] = payloadLen % 256;

    //Ignore 5th and 6th bytes...

    commandIndex ++;
    if(commandIndex > 0xffff)
        commandIndex = 0;
    toSendPacket[6] = commandIndex / 256;
    toSendPacket[7] = commandIndex % 256;
}
/**
 * send packet to the camera tcp server
 * @brief MainWindow::sendPacket
 * @param packet: packet to be sent
 * @param len: length of data in packet
 */
void SonyCam::sendPacket(unsigned int len)
{
    QByteArray temp(toSendPacket , len + 8);
    connector->writeDatagram(temp , QHostAddress(cameraIp), DEFAULT_TARGET_PORTNUM);
}
