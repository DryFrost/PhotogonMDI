#include "mytcp.h"
#include <QDebug>
#include <QCoreApplication>
#include <opencv2/opencv.hpp>
using namespace cv;

MyTCP::MyTCP(QObject *parent) :
    QObject(parent)
{
  connect(&server,SIGNAL(newConnection()),
          this,SLOT(acceptConnection1()));
  server.listen(QHostAddress::Any,8888);
  tmr = new QTimer();
  tmr->setInterval(5000);
  connect(tmr,SIGNAL(timeout()),
          this,SLOT(timeIsOver()));
  StartRead = false;



}
MyTCP::~MyTCP()
{
  server.close();
}
void MyTCP::acceptConnection1()
{
  client = server.nextPendingConnection();
  connect(client,SIGNAL(readyRead()),this,SLOT(startRead()));

}
cv::Mat MyTCP::getImage(){


  return Img;
}

void MyTCP::setStatusBar(QStatusBar *StatusBar)
{
  pStatusBar = StatusBar;
}

cv::Mat qimage_to_mat_ref1(QImage &img, int format)
{
    return cv::Mat(img.height(), img.width(),
            format, img.bits(), img.bytesPerLine());
}
cv::Mat3b QImage2Mat(const QImage &src) {
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

void MyTCP::timeIsOver(){
  QBuffer buffer(&bufferAll);
  if(bufferAll.isEmpty())
    return;
  QImage myImage;
  myImage.load(&buffer,"PNG");
  //bool Sat = true;
  //emit finishedF(Sat);
  Img = QImage2Mat(myImage);
    imwrite("/Users/dnguyen/desktop/LookMomICanDoIt.png",Img);
  pStatusBar->showMessage("Image Recieved");
  //qDebug() << "Image is Got";
  StartRead = false;
  client->close();
}
void MyTCP::startRead(){
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
  qDebug() << "Total: " << bufferAll.size();
}
