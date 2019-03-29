#include "mytcpside.h"
#include <QDebug>
#include <QCoreApplication>
#include <opencv2/opencv.hpp>
using namespace cv;

MyTCPSide::MyTCPSide(QObject *parent) :
    QObject(parent)
{
  connect(&server,SIGNAL(newConnection()),
          this,SLOT(acceptConnection1()));
  server.listen(QHostAddress::Any,8887);
  tmr = new QTimer();
  tmr->setInterval(18000);
  connect(tmr,SIGNAL(timeout()),
          this,SLOT(timeIsOver()));
  StartRead = false;



}
MyTCPSide::~MyTCPSide()
{
  server.close();
}
void MyTCPSide::acceptConnection1()
{
  client = server.nextPendingConnection();
  connect(client,SIGNAL(readyRead()),this,SLOT(startRead()));

}
cv::Mat MyTCPSide::getImage(){


  return Img;
}

void MyTCPSide::setStatusBar(QStatusBar *StatusBar)
{
  pStatusBar = StatusBar;
}

cv::Mat3b QImage2MatSide(const QImage &src) {
  unsigned int height = src.height();
  unsigned int width = src.width();

  cv::Mat3b dest(height, width);
  for (unsigned int y = 0; y < height; ++y) {
    cv::Vec3b *destrow = dest[y];
    for (unsigned int x = 0; x < width; ++x) {
      QRgb pxl = src.pixel(x, y);
      destrow[x] = cv::Vec3b(qBlue(pxl), qGreen(pxl), qRed(pxl));
    }
  }
  return dest;
}

void MyTCPSide::timeIsOver(){
  QBuffer buffer(&bufferAll);
  if(bufferAll.isEmpty())
    return;
  QImage myImage;
  myImage.load(&buffer,"PNG");
  //bool Sat = true;
  //emit finishedF(Sat);
  Img = QImage2MatSide(myImage);
    imwrite("/Users/dnguyen/desktop/LookMomICanDoItFromTheSide.png",Img);
  pStatusBar->showMessage("Image Side Recieved");
  //qDebug() << "Image Side Got";
  StartRead = false;
  client->close();
}
void MyTCPSide::startRead(){
  if(!StartRead){
      tmr->start();
      bufferAll.clear();
      StartRead = true;
    }
  QByteArray ba;
  while(client->bytesAvailable())
    {
      ba = client->readAll();
      bufferAll.append(ba);
    }
  qDebug() << "Total Side: " << bufferAll.size();
}
