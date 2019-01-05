#include "worker.h"

#include <QDebug>
#include <QThread>
#include <QTime>
#include <utility>
Worker::Worker(QString path, int id, bool b) : filepath(std::move(std::move(path))), index(id), Stop(b)
{

}

Worker::~Worker()
= default;

void Worker::readVideo(const QString path)
{
    if (path.length() > 0)
        filepath = path;

    cv::VideoCapture cap(0);


    cv::Mat frame;
    while (Stop=true)
    {

        cap >> frame;
        if (frame.empty())
        {
            frame = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));
            emit frameFinished(frame, index);
            break;
        }




        emit frameFinished(frame.clone(), index);


        QThread::msleep(100);
    }


    emit finished(index);
}

