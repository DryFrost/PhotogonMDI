#include "workerdisplayfront.h"

#include <QDebug>
#include <QThread>
#include <QTime>
#include <utility>
#include <zbar.h>
using namespace cv;
using namespace zbar;

WorkerDisplayFront::WorkerDisplayFront()
{
  _working = true;
  _abort = false;
  path = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));
}

WorkerDisplayFront::~WorkerDisplayFront()
= default;

void WorkerDisplayFront::abort()
{
  mutex.lock();
  if (_working){
      _abort = true;
      qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    }
  mutex.unlock();
}
void WorkerDisplayFront::updateFrm(cv::Mat frm){
  mutex.lock();
  path=frm.clone();
  mutex.unlock();
}
typedef struct{
  std::vector <Point> location;
} decodedObject;

void decode(Mat &im, std::vector<decodedObject>&decodedObjects){
  ImageScanner scanner;
  scanner.set_config(ZBAR_QRCODE,ZBAR_CFG_ENABLE,1);
  Mat imGray;
  cvtColor(im,imGray,CV_BGR2GRAY);
  Image image(im.cols,im.rows,"Y800",(uchar *)imGray.data,im.cols*im.rows);
  int n = scanner.scan(image);
  for(Image::SymbolIterator symbol = image.symbol_begin();symbol != image.symbol_end();++symbol){
      decodedObject obj;
      for (int i = 0; i < symbol->get_location_size();i++) {
          obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }
      decodedObjects.push_back(obj);

    }

}
Mat displayQR(Mat &im, std:: vector<decodedObject>&decodedObjects){
  Mat qrDisp=cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));
  qrDisp = im;
  for(int i = 0; i < decodedObjects.size();i++){
      std::vector<Point> points = decodedObjects[i].location;
      std::vector<Point> hull;
      if(points.size()>4)
        convexHull(points,hull);
      else {
          hull = points;
        }
      int n = hull.size();
      for(int j = 0; j <n;j++){
          line(qrDisp,hull[j],hull[(j+1)%n],Scalar(255,0,0),3);
        }
    }
  return  qrDisp;

}
void WorkerDisplayFront::readVideo()
{

  cv::Size size(400,300);



    while (1)
    {
        std::vector<decodedObject>decodedObjects;
        cv::Mat frame;
        cv::Mat display;
        mutex.lock();
        frame = path;
        bool abort = _abort;
        //qDebug() <<"Thread"<<thread()->currentThreadId()<<" "<<abort;
        mutex.unlock();

        if(abort){
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            frame = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));
            emit frameFinished(frame);
            break;
          }

        QThread::msleep(100);

        decode(frame,decodedObjects);
        frame = displayQR(path,decodedObjects);
        display = frame;

        cv::resize(display,display,size);
        cv::cvtColor(display,display,cv::COLOR_BGR2RGB);
        emit frameFinished(display);



    }


    emit finished();
}

