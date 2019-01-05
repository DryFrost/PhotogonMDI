#include "displayhistogram.h"
#include "ui_displayhistogram.h"

displayHistogram::displayHistogram(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::displayHistogram)
{
  ui->setupUi(this);
}

displayHistogram::~displayHistogram()
{
  delete ui;
}

void displayHistogram::on_pushButton_clicked()
{
  cameraSelection = ui->comboBox->currentText();

}

void displayHistogram::updateHistogramTop(cv::Mat frame){

  if(cameraSelection=="Top"){
      cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}

void displayHistogram::updateHistogramSide(cv::Mat frame){

  if(cameraSelection=="Side"){
      cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}

void displayHistogram::updateHistogramFront(cv::Mat frame){

  if(cameraSelection=="Front"){
      cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
      QImage qImage((uchar*)frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888);
      ui->label->setPixmap(QPixmap::fromImage(qImage));
    }

}
