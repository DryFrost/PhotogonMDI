#ifndef WORKER_H
#define WORKER_H


#include<QApplication>
#include<opencv2/opencv.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mytcp.h>
class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(QString path, int id, bool b = true);
    ~Worker();
    bool Stop;

public slots:
    void readVideo(QString path = "");

signals:
    // frame and index of label which frame will be displayed
    void frameFinished(cv::Mat frame, int index);

    void finished(int index);

private:
    QString filepath;
    int index;
    MyTCP server;
};

#endif // WORKER_H
