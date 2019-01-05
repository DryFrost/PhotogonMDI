#include "camerasetup.h"
#include "ui_camerasetup.h"

cameraSetup::cameraSetup(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::cameraSetup)
{
  ui->setupUi(this);
}

cameraSetup::~cameraSetup()
{
  delete ui;
}
