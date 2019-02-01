#include "displayfront.h"
#include "ui_displayfront.h"
#include <QDebug>
displayFront::displayFront(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displayFront)
{
  ui->setupUi(this);
  threads[0] = new QThread;
  workers[0] = new WorkerDisplayFront();

  workers[0]->moveToThread(threads[0]);

  connect(workers[0], SIGNAL(frameFinished(cv::Mat)), this, SLOT(displayframe(cv::Mat)));
  connect(threads[0], SIGNAL(started()), workers[0], SLOT(readVideo()));
  connect(workers[0], SIGNAL(finished()), threads[0], SLOT(quit()));
  connect(workers[0], SIGNAL(finished()), workers[0], SLOT(deleteLater()));
  connect(threads[0], SIGNAL(finished()), threads[0], SLOT(deleteLater()));

  threads[0]->start();
}

displayFront::~displayFront()
{
  delete ui;
}

void displayFront::updateRawFrameFront(cv::Mat frame){
  //cv::Size size(400,300);
  //cv::resize(frame,frame,size);
  workers[0]->updateFrm(frame);
}
void displayFront::displayframe(cv::Mat frame){
  QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qImage));
  qDebug() << "HI";
}


