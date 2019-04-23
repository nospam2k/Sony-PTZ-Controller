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
void SonyCameraManager::removeCamera(SonyCam *cam)
{
    if(cameraList.contains(cam))
    {
        cameraList.removeOne(cam);
        //stop everything doing with this cam
        //TODO
        delete cam;
    }
}
void SonyCameraManager::readyToRead()
{
    while (receiver->hasPendingDatagrams()) {
        QNetworkDatagram datagram = receiver->receiveDatagram();
        processReceivedData(datagram);
        qDebug()<<datagram.data().toHex()<<datagram.senderAddress().toString();
        //process received datagram
        //TODO
    }
}
void SonyCameraManager::processReceivedData(QDataStream *datagram)
{
    //check ip address is current active camera
    //TODO
    QString command;
    int commandNum;
    //parse data
    //TODO


}

