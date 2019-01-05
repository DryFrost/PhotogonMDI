#include "displayside.h"
#include "ui_displayside.h"

displaySide::displaySide(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displaySide)
{
  ui->setupUi(this);
}

displaySide::~displaySide()
{
  delete ui;
}

void displaySide::updateRawFrameSide(cv::Mat frame){
  cv::Size size(400,300);
  cv::resize(frame,frame,size);
  cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
  QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qImage));

}
