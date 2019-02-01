#ifndef WORKERDISPLAYFRONT_H
#define WORKERDISPLAYFRONT_H

#include <QMutex>
#include <QObject>
#include <opencv2/opencv.hpp>

class WorkerDisplayFront : public QObject
{
    Q_OBJECT
public:
    WorkerDisplayFront();
    void abort();
    void updateFrm(cv::Mat frm);
    ~WorkerDisplayFront();


public slots:
    void readVideo();

signals:
    // frame and index of label which frame will be displayed
    void frameFinished(cv::Mat frame);
    void finished();

private:
    cv::Mat path;
    bool _abort;
    bool _working;
    QMutex mutex;
};

#endif // WORKERDISPLAYFRONT_H
