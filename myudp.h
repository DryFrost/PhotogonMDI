#ifndef MYUDP_H
#define MYUDP_H


#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <QNetworkInterface>
class QUdpSocket;
class MyUDP : public QObject
{
    Q_OBJECT
public:
    explicit MyUDP(QObject *parent = 0);
    void deviceDiscover();
signals:
    void updateList(QString IP);
public slots:
    void readyRead();
private:
    QUdpSocket *socket= nullptr;
    QHostAddress localAddress;
    QHostAddress bcastAddress;


};


#endif // MYUDP_H
