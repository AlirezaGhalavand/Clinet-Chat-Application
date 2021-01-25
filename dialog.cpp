#include "dialog.h"
#include "ui_dialog.h"
#include <QDateTime>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //Start Button enability
    ui->btnConnect->setEnabled(false);

    client = new Client(this);
    connect(client,SIGNAL(connectionStateChanged(QAbstractSocket::SocketState)),this,SLOT(connectionStateChanged(QAbstractSocket::SocketState)));
    connect(client,SIGNAL(clientConnected()),this,SLOT(clientConnected()));
    connect(client,SIGNAL(clientDisconnected()),this,SLOT(clientDisconnected()));
    connect(client,SIGNAL(reportError(QAbstractSocket::SocketError,QString)),this,SLOT(reportError(QAbstractSocket::SocketError,QString)));    
    connect(client,SIGNAL(clientReadyRead(QString)),this,SLOT(showMessage(QString)));

    //ui signals & slots
    connect(ui->txtPort,SIGNAL(textChanged(const QString &)),this,SLOT(portChanged()));
    connect(ui->txtIp,SIGNAL(textChanged(const QString &)),this,SLOT(portChanged()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnConnect_clicked()
{
    QString ipAddress = ui->txtIp->text();
    QString port = ui->txtPort->text();

    client->connectToHost(ipAddress,port.toUShort());

    ui->textEdit->append("<font color=\"green\"><b>Connection Info: </b></font>" + ipAddress + " : " + port);
}

void Dialog::connectionStateChanged(QAbstractSocket::SocketState socketState)
{
    QMetaEnum state = QMetaEnum::fromType<QAbstractSocket::SocketState>();

    ui->textEdit->append("<font color=\"green\"><b>State: </b></font>" + (QString)state.valueToKey(socketState));
}

void Dialog::clientConnected()
{
    ui->textEdit->append("<font color=\"blue\"><b>Connected to the host!</b></font>");

    QPalette pal = ui->btnConnect->palette();
    pal.setColor(QPalette::Button, QColor(Qt::green));
    ui->btnConnect->setAutoFillBackground(true);
    ui->btnConnect->setPalette(pal);
    ui->btnConnect->update();

    getCurrentDateTime();
    ui->textEdit->append("");
    ui->txtIp->clear();
    ui->txtPort->clear();
}

void Dialog::clientDisconnected()
{
    ui->textEdit->append("<font color=\"blue\">No Connection!</b></font>");
    getCurrentDateTime();
    ui->textEdit->append("");

    QPalette pal = ui->btnConnect->palette();
    pal.setColor(QPalette::Button, QColor(Qt::white));
    ui->btnConnect->setAutoFillBackground(true);
    ui->btnConnect->setPalette(pal);
    ui->btnConnect->update();
}

void Dialog::reportError(QAbstractSocket::SocketError socketError, QString errorString)
{
    ui->textEdit->append("<font color=\"red\"><b>Error: </b></font>" + (QString)socketError + " " + errorString);
    ui->textEdit->append("");
}

void Dialog::portChanged()
{
    //Start Button enability
    if(!ui->txtIp->text().isEmpty() && !ui->txtPort->text().isEmpty())
        ui->btnConnect->setEnabled(true);
    else
        ui->btnConnect->setEnabled(false);
}

void Dialog::showMessage(QString message)
{
    ui->textEdit->append("<font color=\"green\"><b>server: </b></font>" + message);
    getCurrentDateTime();
    ui->textEdit->append("");
}

void Dialog::on_btnSend_clicked()
{
    QByteArray m = ui->lineEdit->text().toLatin1();
    bool messageResult = client->sendMessage(m);
    if(messageResult)
    {
        ui->textEdit->append(m);
        getCurrentDateTime();
        ui->textEdit->append("");
        ui->lineEdit->clear();
    }
    else
        ui->textEdit->append("<font color=\"blue\"><b>No Connection!</b></font>");
}

void Dialog::getCurrentDateTime()
{
    ui->textEdit->append(QString ("<font color=\"red\"><small>%1</small></font>").arg(QDateTime::currentDateTime().toString()));
}
