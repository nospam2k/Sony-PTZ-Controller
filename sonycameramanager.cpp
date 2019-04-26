#include "sonycameramanager.h"
#include <QDebug>
SonyCameraManager::SonyCameraManager(QObject *parent) : QObject(parent)
{
    initUdpSocket();
}
SonyCameraManager::~SonyCameraManager()
{
    for(int i = 0 ; i < cameraList.length() ; i ++)
    {
        SonyCam *temp = cameraList.at(0);
        cameraList.removeAt(0);
        //stop everything doing with this cam
        //TODO
        delete temp;
    }

    if(receiver->isOpen())
        receiver->close();
    delete receiver;
}
void SonyCameraManager::initUdpSocket()
{
    receiver = new QUdpSocket();
    receiver->bind(DEFAULT_RECEIVE_PORTNUM);
    connect(receiver , SIGNAL(readyRead()) , this , SLOT(readyToRead()));
}
void SonyCameraManager::addCamera(SonyCam *cam)
{
    cameraList.append(cam);
    //add in the file

}
void SonyCameraManager::removeCamera(SonyCam *cam)
{
    if(cameraList.contains(cam))
    {
        cameraList.removeOne(cam);
        //remove from the file
        //TODO
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
void SonyCameraManager::processReceivedData(QNetworkDatagram datagram)
{
    //check ip address is current active camera
    //TODO
    QString command;
    int commandNum;
    //parse data
    //TODO

}

