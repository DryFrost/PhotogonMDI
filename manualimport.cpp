#include "manualimport.h"
#include "ui_manualimport.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QSettings>
#include <manualimportselected.h>
#include <opencv2/opencv.hpp>
using namespace cv;
ManualImport::ManualImport(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ManualImport)
{
  ui->setupUi(this);
  show();

}

ManualImport::~ManualImport()
{
  delete ui;
}

void ManualImport::on_pushButton_clicked()
{

  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               "/home",
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("ProjectDirManual",QVariant::fromValue(dir));
  new ManualImportSelected();




}


void ManualImport::on_pushButton_3_clicked()
{

  for(int i = 0; i<FileName.count(); i++){
      Mat Current = imread(FileName[i].toUtf8().constData());
    }


}

void ManualImport::on_pushButton_2_clicked()
{
  QSettings internal("internal.ini",QSettings::IniFormat);
  FileName = internal.value("ManualFileDirs").value<QStringList>();
  ui->listWidget->addItems(FileName);
}

void ManualImport::on_pushButton_4_clicked()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               "/home",
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);

}

void ManualImport::on_pushButton_5_clicked()
{

}
