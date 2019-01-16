#include "myudp.h"

MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Binding UDP Socket";
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any,5000);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}
void MyUDP::deviceDiscover(QString command){
    QByteArray data;
    data.append(command);
    qDebug() << "Sending broadcast";
    socket->writeDatagram(data,QHostAddress::Broadcast,5000);
}
void MyUDP::readyRead(){
    QByteArray buffer;
    buffer.resize(socket->pendingDatagramSize());
    QHostAddress sender;
    quint16 sender_port;
    socket->readDatagram(buffer.data(),buffer.size(),&sender,&sender_port);
    qDebug() << "Message from " << sender << "port " << sender_port;
    qDebug() << "Msg: " << buffer;
    if(QString(buffer).compare("PI")==0){
        emit(updateList(sender.toString()));
        qDebug() << "Emitting change";
    }
}

