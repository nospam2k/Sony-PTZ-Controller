#include "sonycam.h"
#include "sonyptzapp.h"
SonyCam::SonyCam(QString ip, unsigned int portNum , QString cameraName)
{
    isConnected = false;
    isWaitingReply = false;
    this->cameraIp = ip;
    this->portNum = portNum;
    this->cameraName = cameraName;
    initUdpConnector();
    initTimer();
}
SonyCam::~SonyCam()
{
    stopLooping();
    disconnectFromCamera();
    delete timer;    
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
    emit cameraConnected();
    //initiate
    initMessaging();
}
void SonyCam::onClientDisconnected()
{
    isConnected = false;
    stopLooping();
    emit cameraDisconnected();
}
void SonyCam::onError()
{
    emit cameraError();
}
void SonyCam::onTimerOneSec()
{
    emit looping(this);
    if(isWaitingReply)
        return;
    //increase counter
    counter ++;
    qDebug()<<counter;


    if(counter >= (2 * waiteTime))
    {

        counter = 0;
        //call next preset
        if((curPresetIndex) < presetLoop.length())
        {
            callPreset(presetLoop.at(curPresetIndex).presetNum);
            curPresetIndex ++;
            if(curPresetIndex >= presetLoop.length())
                curPresetIndex = 0;
        }
        else
        {
            curPresetIndex = 0;
        }

    }
}
void SonyCam::onReceiveReply()
{
    //call next command in command que
    isWaitingReply = false;
    if(commandQue.length() > 0)
        sendPacket();
}
void SonyCam::onReceiveACK()
{

}
void SonyCam::connectToCamera()
{
    qDebug()<<cameraIp;
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
    emit cameraIpChanged();
}
void SonyCam::setCameraName(QString name)
{
    this->cameraName = name;
    emit cameraNameChanged();
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
void SonyCam::initMessaging()
{
    commandIndex = 0;
    createMessageHeader(CONTROL_COMMAND_HEAD , 1);
    toSendPacket[8] = 0x01;
    addInTheQue(1);

}
void SonyCam::setPresetSpeed(unsigned int presetNum , unsigned int speed)
{
    //81 01 7E 01 0B pp qq FF
    createMessageHeader(COMMAND_HEAD , 8);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x7e;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = 0x0b;
    toSendPacket[13] = (char)(presetNum % 255);//max preset number 64
    toSendPacket[14] = (char)(speed % 255);//max speed 18
    toSendPacket[15] = 0xff;
    addInTheQue(8);

}
void SonyCam::setPreset(unsigned int presetNum)
{
    //8x 01 04 3F 01 pp FF
    createMessageHeader(COMMAND_HEAD , 7);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x3f;
    toSendPacket[12] = 0x01;
    toSendPacket[13] = (char)(presetNum % 255);
    toSendPacket[14] = 0xff;
    addInTheQue(7);

}
void SonyCam::callPreset(unsigned int presetNum)
{
    //8x 01 04 3F 02 pp FF
    //8x 01 04 3F 01 pp FF
    createMessageHeader(COMMAND_HEAD , 7);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x3f;
    toSendPacket[12] = 0x02;
    toSendPacket[13] = (char)(presetNum % 255);
    toSendPacket[14] = 0xff;
    addInTheQue(7);

}
void SonyCam::addPreset(PRESET preset)
{
    presetLoop.append(preset);
}
void SonyCam::removePreset(int presetIndex)
{
    presetLoop.removeAt(presetIndex);
}
void SonyCam::replacePreset(PRESET preset, int presetIndex)
{
    presetLoop.replace(presetIndex , preset);
}
QList<PRESET> SonyCam::getPresetList()
{
    return this->presetLoop;
}
bool SonyCam::isLooping()
{
    return timer->isActive();
}
void SonyCam::startLooping()
{
    if(timer->isActive())
        return;
    //start timer
    timer->start(500);//start looping every 0.5 sec

    //call first preset
    if(presetLoop.length() > 0)
        callPreset(presetLoop.at(0).presetNum);
    counter = 0;
    curPresetIndex = 1;
    emit loopingStarted(this);
}
void SonyCam::stopLooping()
{
    //stop timer
    timer->stop();
    //delete all message
    for(int i = 0 ; i < commandQue.length() ; i ++)
    {
        COMMAND *temp = commandQue.at(0);
        commandQue.removeAt(0);
        delete temp;
    }
    emit loopingStopped(this);
}
void SonyCam::setCallPresetSpeed(unsigned int speed)
{
    this->callPresetSpeed = speed;
    for(int i = 0 ; i < presetLoop.length() ; i ++)
    {
        setPresetSpeed(presetLoop.at(i).presetNum , speed);
    }
}
void SonyCam::setWaitTime(unsigned int seconds)
{
    this->waiteTime = seconds;
}
unsigned int SonyCam::getCallPresetSpeed()
{
    return this->callPresetSpeed;
}
unsigned int SonyCam::getWaitTime()
{
    return this->waiteTime;
}
void SonyCam::moveOneLeft(unsigned int panSpeed)
{
    //81 01 06 01 vv ww 01 03 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = (char)(panSpeed % 255);//max pan speed 18
    toSendPacket[13] = 5;//default tilt speed
    toSendPacket[14] = 0x01;
    toSendPacket[15] = 0x03;
    toSendPacket[16] = 0xff;
    addInTheQue(9);

}
void SonyCam::moveOneRight(unsigned int panSpeed)
{
    //81 01 06 01 vv ww 02 03 FF
    createMessageHeader(COMMAND_HEAD , 9);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x06;
    toSendPacket[11] = 0x01;
    toSendPacket[12] = (char)(panSpeed % 255);//max pan speed 18
    toSendPacket[13] = 5;//default tilt speed
    toSendPacket[14] = 0x02;
    toSendPacket[15] = 0x03;
    toSendPacket[16] = 0xff;
    addInTheQue(9);

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
    toSendPacket[13] = (char)(tiltSpeed % 255);//max tilt speed 18
    toSendPacket[14] = 0x03;
    toSendPacket[15] = 0x01;
    toSendPacket[16] = 0xff;
    addInTheQue(9);

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
    toSendPacket[13] = (char)(tiltSpeed % 255);//max tilt speed 18
    toSendPacket[14] = 0x03;
    toSendPacket[15] = 0x02;
    toSendPacket[16] = 0xff;
    addInTheQue(9);

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
    addInTheQue(5);

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
    addInTheQue(6);

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
    addInTheQue(6);

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
    addInTheQue(6);

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
    addInTheQue(6);

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
    addInTheQue(6);

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
    addInTheQue(6);

}
void SonyCam::stopZooming()
{
    //81 01 04 07 00 FF
    createMessageHeader(COMMAND_HEAD , 6);
    toSendPacket[8] = 0x81;
    toSendPacket[9] = 0x01;
    toSendPacket[10] = 0x04;
    toSendPacket[11] = 0x07;
    toSendPacket[12] = 0x00;
    toSendPacket[13] = 0xff;
    addInTheQue(6);

}
void SonyCam::stopMoving()
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
    addInTheQue(9);

}
QString SonyCam::getErrorMessage()
{
    return connector->errorString();
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
void SonyCam::sendPacket()
{
    if(!isConnected)
        return;
    COMMAND* command = commandQue.at(0);
    QByteArray temp(command->commandByte , command->commandLength);
    connector->write(temp);
    int commandType = (static_cast<unsigned int>(command->commandByte[0])) * 0x0100 + (static_cast<unsigned int>(command->commandByte[1]));
    int comIndex = (static_cast<unsigned int>(command->commandByte[6])) * 0x0100 + (static_cast<unsigned int>(command->commandByte[7]));
    emit messageSent(commandType , temp.toHex() , comIndex , this);
    commandQue.removeFirst();
    delete command;
    isWaitingReply = true;
}
void SonyCam::addInTheQue(unsigned int len)
{
    if(!isConnected)
        return;
    COMMAND *command = new COMMAND;
    memcpy(command->commandByte , toSendPacket , len + 8);
    command->commandLength = len + 8;
    commandQue.append(command);
    if(!isWaitingReply && commandQue.length() > 0)
        sendPacket();

}
