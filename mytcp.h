#ifndef MYTCP_H
#define MYTCP_H
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
class MyTCP: public QObject
{
  Q_OBJECT
public:
  explicit MyTCP(QObject *parent = 0);
  void setStatusBar(QStatusBar* StatusBar);
  cv::Mat getImage();
  ~MyTCP();
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

#endif // MYTCP_H
