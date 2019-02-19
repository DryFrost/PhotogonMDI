#include "manualroi.h"
#include "ui_manualroi.h"
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <opencv2/opencv.hpp>
using namespace cv;

ManualROI::ManualROI(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ManualROI)
{
  ui->setupUi(this);
  show();
  timer = new QTimer(this);
  QSettings internal("internal.ini",QSettings::IniFormat);
  x1 = internal.value("x1Front").value<int>();
  x2 = internal.value("x2Front").value<int>();
  y1 = internal.value("y1Front").value<int>();
  y2 = internal.value("y2Front").value<int>();
  ui->x1->setValue(x1);
  ui->x2->setValue(x2);
  ui->y1->setValue(y1);
  ui->y2->setValue(y2);

}

ManualROI::~ManualROI()
{
  delete ui;
}

void ManualROI::on_load_clicked()
{
  FilePath = QFileDialog::getOpenFileName(this, tr("Files"), QDir::currentPath(), tr("*.jpg *.png"));
  CurrentFrame = imread(FilePath.toUtf8().constData());
  connect(timer,SIGNAL(timeout()),this,SLOT(update_window()));
  timer->start(20);
}

void ManualROI::update_window(){
  x1 = ui->x1->value();
  x2 = ui->x2->value();
  y1 = ui->y1->value();
  y2 = ui->y2->value();

  cv::rectangle(CurrentFrame,Point(x1,y1),Point(x2,y2),Scalar(255,255,0),4,LINE_8,0);

  Size size(400,225);
  cv::resize(CurrentFrame,CurrentFrame,size);
  cvtColor(CurrentFrame,CurrentFrame,CV_BGR2RGB);
  QImage qt_image = QImage((const unsigned char*)(CurrentFrame.data),CurrentFrame.cols,CurrentFrame.rows,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qt_image));
}

void ManualROI::on_save_clicked()
{
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("x1Front",QVariant::fromValue(x1));
  internal.setValue("y1Front",QVariant::fromValue(y1));
  internal.setValue("x2Front",QVariant::fromValue(x2));
  internal.setValue("y2Front",QVariant::fromValue(y2));
  close();
}

void ManualROI::on_cancel_clicked()
{
    close();
}
