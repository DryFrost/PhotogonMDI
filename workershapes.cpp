#include "workershapes.h"

#include <QDebug>
#include <QThread>
#include <QTime>
#include <utility>
workerShapes::workerShapes(QString path) : filepath(std::move(std::move(path)))
{
  _working = true;
  _abort = false;
}

workerShapes::~workerShapes()
= default;

void workerShapes::abort()
{
  mutex.lock();
  if (_working){
      _abort = true;
      qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    }
  mutex.unlock();
}

void workerShapes::readVideo(const QString path)
{
    if (path.length() > 0)
        filepath = path;

    cv::VideoCapture cap(0);


    cv::Mat frame;
    while (1)
    {
        mutex.lock();
        bool abort = _abort;
        qDebug() <<"Thread"<<thread()->currentThreadId()<<" "<<abort;
        mutex.unlock();

        if(abort){
            qDebug()<<"Aborting worker process in Thread "<<thread()->currentThreadId();
            frame = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));
            emit frameFinished(frame);
            break;
          }

        QThread::msleep(100);

        cap >> frame;
        if (frame.empty())
        {
            frame = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));
            emit frameFinished(frame);
            break;
        }

        emit frameFinished(frame.clone());

    }


    emit finished();
}

