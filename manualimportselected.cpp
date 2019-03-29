#include "manualimportselected.h"
#include "ui_manualimportselected.h"
#include <QDirIterator>
#include <QSettings>

ManualImportSelected::ManualImportSelected(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ManualImportSelected)
{
  ui->setupUi(this);
  show();
  QSettings internal("internal.ini",QSettings::IniFormat);
  QString dir = internal.value("ProjectDirManual").value<QString>();
  QDirIterator it(dir, QStringList() << "*.png", QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext()){
      QFile f(it.next());
      f.open(QIODevice::ReadOnly);
      ui->listWidget->addItem(f.fileName());
    }
  QListWidgetItem* item = 0;
  for(int i = 0; i < ui->listWidget->count(); ++i){
      item = ui->listWidget->item(i);
      item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
      item->setCheckState(Qt::Checked);
    }

}

ManualImportSelected::~ManualImportSelected()
{
  delete ui;
}

void ManualImportSelected::on_pushButton_clicked()
{
  QStringList FileNameShortList;
  QListWidgetItem* item = 0;
      for(int i = 0; i < ui->listWidget->count(); ++i){
          item = ui->listWidget->item(i);
          if(item->checkState() == Qt::Checked){
              FileNameShortList << ui->listWidget->item(i)->text();
            }
      }
      QSettings internal("internal.ini",QSettings::IniFormat);
      internal.setValue("ManualFileDirs",QVariant::fromValue(FileNameShortList));
      close();
}


