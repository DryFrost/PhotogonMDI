#include "connectcameras.h"
#include "ui_connectcameras.h"
#include <QSettings>

connectCameras::connectCameras(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::connectCameras)
{
  ui->setupUi(this);
}

connectCameras::~connectCameras()
{
  delete ui;
}

void connectCameras::on_pushButton_clicked()
{

  QString front = ui->label->text();
  QString side = ui->label_2->text();
  QString top = ui->label_3->text();

  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("FrontCameraIP",QVariant::fromValue(front));
  internal.setValue("SideCameraIP",QVariant::fromValue(side));
  internal.setValue("TopCameraIP",QVariant::fromValue(top));
  close();

}
