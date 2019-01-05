#include "displayfront.h"
#include "ui_displayfront.h"

displayFront::displayFront(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displayFront)
{
  ui->setupUi(this);
}

displayFront::~displayFront()
{
  delete ui;
}

void displayFront::updateRawFrameFront(cv::Mat frame){
  cv::Size size(400,300);
  cv::resize(frame,frame,size);
  cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
  QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qImage));
}
