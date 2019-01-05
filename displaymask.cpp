#include "displaymask.h"
#include "ui_displaymask.h"

displayMask::displayMask(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displayMask)
{
  ui->setupUi(this);
}

displayMask::~displayMask()
{
  delete ui;
}

void displayMask::on_pushButton_clicked()
{
  cameraSelection = ui->comboBox->currentText();
}

void displayMask::updateMaskTop(cv::Mat frame){

  if(cameraSelection=="Top"){
      cv::Size size(400,225);
      cv::resize(frame,frame,size);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Indexed8);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}

void displayMask::updateMaskFront(cv::Mat frame){

  if(cameraSelection=="Front"){
      cv::Size size(400,225);
      cv::resize(frame,frame,size);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Indexed8);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }
}

void displayMask::updateMaskSide(cv::Mat frame){

  if(cameraSelection=="Side"){
      cv::Size size(400,225);
      cv::resize(frame,frame,size);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Indexed8);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}
