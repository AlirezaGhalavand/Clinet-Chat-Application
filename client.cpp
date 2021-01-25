#include "client.h"

using namespace std;

Client::Client(QObject *parent) : QObject(parent)
{
    connect(&socket,&QTcpSocket::connected,this,&Client::connected);
    connect(&socket,&QTcpSocket::disconnected,this,&Client::disconnected);
    connect(&socket,&QTcpSocket::stateChanged,this,&Client::stateChanged);
    connect(&socket,&QTcpSocket::readyRead,this,&Client::readyRead);
    connect(&socket,QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this,&Client::error);
}

void Client::connectToHost(QString hostName, quint16 port)
{
    if(!socket.isOpen()) disconnected();

    socket.connectToHost(hostName,port);
}

void Client::disconnected()
{
    connectionFlag = 0;
    emit clientDisconnected();
    socket.close();
}

void Client::connected()
{
    connectionFlag = 1;
    emit clientConnected();
}

void Client::error(QAbstractSocket::SocketError socketError)
{
    emit reportError(socketError,socket.errorString());
}

void Client::stateChanged(QAbstractSocket::SocketState socketState)
{
    emit connectionStateChanged(socketState);
}

void Client::readyRead()
{
    QString message = (QString)socket.readAll();

    emit clientReadyRead(message);
}

bool Client::sendMessage(QByteArray message)
{
    if(connectionFlag)
    {
        socket.write(message);
        return 1;
    }

    else
        return 0;
}
