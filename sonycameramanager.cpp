#include "sonycameramanager.h"
#include "sonyptzapp.h"
#include <QDebug>
SonyCameraManager::SonyCameraManager(QObject *parent) : QObject(parent)
{
    initUdpSocket();
    curCam = nullptr;
}
SonyCameraManager::~SonyCameraManager()
{
    for(int i = 0 ; i < cameraList.length() ; i ++)
    {
        SonyCam *temp = cameraList.at(0);
        cameraList.removeAt(0);
        delete temp;
    }

    if(receiver->isOpen())
        receiver->close();
    delete receiver;
}
void SonyCameraManager::initUdpSocket()
{
    receiver = new QUdpSocket();
    if(!receiver->bind(DEFAULT_RECEIVE_PORTNUM))
    {
        App()->showMessage("Can not open port. Maybe the same application is running already.");
    }
    connect(receiver , SIGNAL(readyRead()) , this , SLOT(readyToRead()));
}
void SonyCameraManager::loadCameras()
{
    for(int i = 0 ; i < App()->getAppSettings()->getCameraCount() ; i ++)
    {
        addCamera(App()->getAppSettings()->getCamera(i));
    }
}
void SonyCameraManager::addCamera(SonyCam *cam)
{
    cameraList.append(cam);
    connect(cam, &SonyCam::cameraConnected , [this , cam]{
        if(cam == this->curCam)
            emit this->curCamConnected();
    });
    connect(cam, &SonyCam::cameraDisconnected , [this , cam]{
        if(cam == this->curCam)
            emit this->curCamDisconnected();
    });
    connect(cam, &SonyCam::cameraError , [this , cam]{
        if(cam == this->curCam)
            emit this->curCamError();
    });
    connect(cam , &SonyCam::cameraIpChanged , [this , cam]{
        if(cam == this->curCam)
            emit this->curCamIpChanged();
    });
    connect(cam , SIGNAL(messageSent(int,QString,int,SonyCam*)) , this , SLOT(onMessageSent(int,QString,int,SonyCam*)));
    connect(cam , SIGNAL(loopingStarted(SonyCam*)) , this , SLOT(onLoopingStarted(SonyCam*)));
    connect(cam , SIGNAL(loopingStopped(SonyCam*)) , this , SLOT(onLoopingStopped(SonyCam*)));
    emit cameraAdded();
}
void SonyCameraManager::removeCamera(SonyCam *cam)
{
    if(cameraList.contains(cam))
    {
        cameraList.removeOne(cam);
        delete cam;
    }
}
SonyCam* SonyCameraManager::getCam(int index)
{
    if(index < cameraList.length())
        return cameraList.at(index);
    else
        return nullptr;
}
int SonyCameraManager::getCamCount()
{
    return cameraList.length();
}
SonyCam* SonyCameraManager::getCurCam()
{
    return curCam;
}
void SonyCameraManager::setCurCam(int index)
{
    if(index < cameraList.length())
    {
        this->curCam = cameraList.at(index);
        emit curCamChanged();
    }
}
void SonyCameraManager::setCurCam(SonyCam *cam)
{
    this->curCam = cam;
    emit curCamChanged();
}
int SonyCameraManager::getCamIndex(SonyCam *cam)
{
    return cameraList.indexOf(cam);
}
void SonyCameraManager::readyToRead()
{
    while (receiver->hasPendingDatagrams()) {
        QNetworkDatagram datagram = receiver->receiveDatagram();
        processReceivedData(datagram);
        qDebug()<<datagram.data().toHex()<<datagram.senderAddress().toString();
    }
}
//char *readBuffer = new char[MAX_BUFFER_SIZE];
//int currentPos = -1;//current byte position -1:did not find the preamble
void SonyCameraManager::processReceivedData(QNetworkDatagram datagram)
{

    //check ip address is current active camera
    if(curCam == nullptr)
        return;


    //parse data

    QByteArray temp = datagram.data();
    if(temp.length() < 8)
        return;
    int type = (static_cast<unsigned int>(temp.at(0))) * 0x0100 + (static_cast<unsigned int>(temp.at(1)));

    QString bytes = temp.mid(8).toHex();

    int commandNum = (static_cast<unsigned int>(temp.at(6))) * 0x0100 + (static_cast<unsigned int>(temp.at(7)));


    if(getCam(datagram.senderAddress()) != nullptr)
    {
        switch(type)
        {
        case REPLY_HEAD:
            if(bytes == "9041ff" || bytes == "9042ff")
                getCam(datagram.senderAddress())->onReceiveACK();
            if(bytes == "9051ff" || bytes == "9052ff")
                getCam(datagram.senderAddress())->onReceiveReply();
            if(bytes == "906002ff" || bytes == "906003ff" || bytes == "906141ff" || bytes == "906241ff")//syntax, command bufferfull, command not executable
                getCam(datagram.senderAddress())->onReceiveReply();

            break;
        case CONTROL_COMMAND_REPLY_HEAD:
            getCam(datagram.senderAddress())->onReceiveReply();
        }
    }
    if(datagram.senderAddress().isEqual(QHostAddress(curCam->getCameraIp()) , QHostAddress::ConvertV4MappedToIPv4))
        buildReportData(type , bytes , commandNum);

}
void SonyCameraManager::buildReportData(int type , QString bytes , int comIndex)
{
    QString temp;
    switch(type)
    {
    case COMMAND_HEAD:
    case CONTROL_COMMAND_HEAD:
    case INQUERY_HEAD:
        temp += "Tx: ";
        break;
    case CONTROL_COMMAND_REPLY_HEAD:
    case REPLY_HEAD:
        temp += "Rx: ";
        break;
    }
    temp += bytes;
    temp += " " + QString::number(comIndex);
    qDebug()<<temp;
    emit reportData(temp);
}
void SonyCameraManager::onMessageSent(int type , QString bytes , int comIndex , SonyCam* cam)
{
    qDebug()<<bytes;
    if(cam != curCam)
        return;
    if(bytes.length() < 16)//2 x 8 = 16 letters for 8 bytes
        return;
    buildReportData(type , bytes.mid(16) , comIndex);
}
SonyCam* SonyCameraManager::getCam(QHostAddress ipAddr)
{
    for(int i = 0 ; i < cameraList.length() ; i ++)
    {
        if(QHostAddress(cameraList.at(i)->getCameraIp()).isEqual(ipAddr , QHostAddress::ConvertV4MappedToIPv4))
            return cameraList.at(i);
    }
    return nullptr;
}
void SonyCameraManager::onLoopingStarted(SonyCam *cam)
{
    qDebug()<<cam;
    qDebug()<<curCam;
    if(cam == curCam)
        emit curCamLoopingStarted();
}
void SonyCameraManager::onLoopingStopped(SonyCam *cam)
{
    if(cam == curCam)
        emit curCamLoopingStopped();
}
