#ifndef LISTGENERATE_H
#define LISTGENERATE_H

#include <QDialog>
#include "treatmentdialog.h"
#include "treatmentnumdialog.h"
#include "treatmentlevelsdialog.h"
#include <QDebug>
#include <QSettings>

namespace Ui {
  class listGenerate;
}

class listGenerate : public QDialog
{
  Q_OBJECT

public:
  explicit listGenerate(QWidget *parent = nullptr);
  ~listGenerate();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

private:
  Ui::listGenerate *ui;
  QString ProjectDir;
  QString ProjectName;
  int numTreatments;
  int numRepeats;
  QStringList treatmentList;
  QStringList treatmentNumList;
  QStringList model;


};

#endif // LISTGENERATE_H
