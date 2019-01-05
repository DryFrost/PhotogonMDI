#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include "qrgenerate.h"
#include "listgenerate.h"
#include "configdialog.h"

namespace Ui {
  class NewProject;
}

class NewProject : public QDialog
{
  Q_OBJECT

public:
  explicit NewProject(QWidget *parent = nullptr);
  ~NewProject();

private slots:
  void on_SetDirectory_clicked();

  void on_Generate_clicked();

  void on_projectName_textChanged(const QString &arg1);

  void on_pushButton_5_clicked();

  void on_Config_clicked();

  void on_SetupCameras_clicked();

  void on_pushButton_4_clicked();

private:
  Ui::NewProject *ui;
  QString ProjectName;
  QString ProjectDir;

};

#endif // NEWPROJECT_H
