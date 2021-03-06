#include "displaytop.h"
#include "ui_displaytop.h"

displayTop::displayTop(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displayTop)
{
  ui->setupUi(this);
  frame = cv::Mat(cv::Size(640, 480), CV_8UC3, cv::Scalar(192, 0, 0));

}

displayTop::~displayTop()
{
  delete ui;
}

void displayTop::updateRawFrameTop(cv::Mat frame){
  cv::Size size(400,300);
  cv::resize(frame,frame,size);
  cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
  QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qImage));

}
