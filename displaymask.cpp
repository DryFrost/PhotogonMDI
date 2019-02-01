#include "displaymask.h"
#include "ui_displaymask.h"
#include <QDebug>
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
      QImage qImage1((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_Indexed8);
      ui->label->setPixmap(QPixmap::fromImage(qImage1));
      qDebug() << "Mask Top";
    }

}

void displayMask::updateMaskFront(cv::Mat frame){
  if(cameraSelection=="Front"){
      cv::Size size(400,225);
      cv::resize(frame,frame,size);
      QImage qImage2((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_Indexed8);
      ui->label->setPixmap(QPixmap::fromImage(qImage2));
      qDebug() << "Mask Front";

    }
}

void displayMask::updateMaskSide(cv::Mat frame){

  if(cameraSelection=="Side"){
      cv::Size size(400,225);
      cv::resize(frame,frame,size);
      QImage qImage3((const unsigned char*)(frame.data),frame.cols,frame.rows,frame.step,QImage::Format_Indexed8);
      ui->label->setPixmap(QPixmap::fromImage(qImage3));
      qDebug() << "Mask Side";

    }

}
