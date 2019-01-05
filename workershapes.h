#ifndef WORKERSHAPES_H
#define WORKERSHAPES_H

#include <QMutex>
#include <QObject>
#include <opencv2/opencv.hpp>

class workerShapes : public QObject
{
    Q_OBJECT
public:
    workerShapes(QString path);
    void abort();
    ~workerShapes();


public slots:
    void readVideo(QString path = "");

signals:
    // frame and index of label which frame will be displayed
    void frameFinished(cv::Mat frame);

    void finished();

private:
    QString filepath;
    bool _abort;
    bool _working;
    QMutex mutex;
};

#endif // WORKERSHAPES_H
