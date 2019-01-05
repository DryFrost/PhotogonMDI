#include "displayshape.h"
#include "ui_displayshape.h"

displayShape::displayShape(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displayShape)
{
  ui->setupUi(this);
}

displayShape::~displayShape()
{
  delete ui;
}

void displayShape::on_pushButton_clicked()
{
  cameraSelection = ui->comboBox->currentText();
}
void displayShape::updateShapeTop(cv::Mat frame){
  if(cameraSelection=="Top"){
      cv::Size size(350,169);
      cv::resize(frame,frame,size);
      cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}
void displayShape::updateShapeFront(cv::Mat frame){
  if(cameraSelection=="Front"){
      cv::Size size(350,169);
      cv::resize(frame,frame,size);
      cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}
void displayShape::updateShapeSide(cv::Mat frame){
  if(cameraSelection=="Side"){
      cv::Size size(350,169);
      cv::resize(frame,frame,size);
      cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }
}
