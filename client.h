#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QMetaEnum>
#include <QAbstractSocket>
#include <iostream>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    bool sendMessage(QByteArray message);

signals:
    void connectionStateChanged(QAbstractSocket::SocketState socketState);
    void clientConnected();
    void clientDisconnected();
    void reportError(QAbstractSocket::SocketError socketError,QString errorString);
    void clientReadyRead(QString message);

public slots:
    void connectToHost(QString hostName, quint16 port);

private slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void readyRead();

private:
    QTcpSocket socket;
    bool connectionFlag;
};

#endif // CLIENT_H
