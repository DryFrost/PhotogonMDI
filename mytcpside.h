#ifndef MYTCPSIDE_H
#define MYTCPSIDE_H


#include <QTcpServer>
#include <QObject>
#include <QtNetwork>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QImage>
#include <QPaintDevice>
#include <opencv2/opencv.hpp>
#include <QStatusBar>
class QTcpServer;
class MyTCPSide: public QObject
{
  Q_OBJECT
public:
  explicit MyTCPSide(QObject *parent = 0);
  void setStatusBar(QStatusBar* StatusBar);
  cv::Mat getImage();
  ~MyTCPSide();
signals:
  void newConnection();
public slots:
  void acceptConnection1();
  void startRead();
  void timeIsOver();
private:
  QTcpServer server;
  QTcpSocket* client;
  QTimer *tmr;
  bool StartRead;
  QByteArray bufferAll;
  cv::Mat Img;
  QStatusBar* pStatusBar;
};

#endif // MYTCPSIDE_H
