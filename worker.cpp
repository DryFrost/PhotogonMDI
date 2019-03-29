#include "worker.h"

#include <QDebug>
#include <QThread>
#include <QTime>
#include <utility>

#define FRAME_HEIGHT 1944
#define FRAME_WIDTH 2592

using namespace cv;

Worker::Worker(QString path, int id, bool b) : filepath(std::move(std::move(path))), index(id), Stop(b)
{

}

Worker::~Worker()
= default;

void Worker::readVideo(const QString path)
{
        Mat img;

        emit frameFinished(img.clone(), index);


        QThread::msleep(10);


    emit finished(index);
}
