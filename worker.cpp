#include "worker.h"

#include <QDebug>
#include <QThread>
#include <QTime>
#include <utility>

#define FRAME_HEIGHT 480
#define FRAME_WIDTH 640

using namespace cv;

Worker::Worker(QString path, int id, bool b) : filepath(std::move(std::move(path))), index(id), Stop(b)
{

}

Worker::~Worker()
= default;

void Worker::readVideo(const QString path)
{
    if (path.length() > 0)
        filepath = "192.168.0.36";

    filepath = "192.168.0.36";
    QByteArray temp = filepath.toLocal8Bit();
    char* serverIP = temp.data();

    int sokt;
    int serverPort;

    serverPort = atoi("4098");



    struct  sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }


    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (::connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
           std::cerr << "connect() failed!" << std::endl;
       }

    Mat img;
    img = Mat::zeros(FRAME_HEIGHT,FRAME_WIDTH,CV_8UC3);
    int imgSize = img.total()*img.elemSize();
    //uchar sockData[imgSize];
    uchar *iptr = img.data;
    int bytes = 0;


    while (1)
    {

        if ((bytes = recv(sokt, iptr, imgSize , MSG_WAITALL)) == -1) {
                    std::cerr << "recv failed, received bytes = " << bytes << std::endl;
                }

        int ptr = 0;

        for(int i = 0; i<img.rows;++i)
                   for(int j = 0; j<img.cols;++j)
                   {
                       img.at<Vec3b>(i,j)= Vec3b(iptr[ptr+0],iptr[ptr+1],iptr[ptr+2]);
                       ptr=ptr+3;
                   }


        emit frameFinished(img.clone(), index);


        QThread::msleep(10);
    }


    emit finished(index);
}
