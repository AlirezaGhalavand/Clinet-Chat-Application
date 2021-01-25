#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void getCurrentDateTime();

private slots:
    void on_btnConnect_clicked();
    void connectionStateChanged(QAbstractSocket::SocketState socketState);
    void clientConnected();
    void clientDisconnected();
    void reportError(QAbstractSocket::SocketError socketError,QString errorString);
    void portChanged();
    void showMessage(QString message);

    void on_btnSend_clicked();

private:
    Ui::Dialog *ui;
    Client *client;
};
#endif // DIALOG_H
