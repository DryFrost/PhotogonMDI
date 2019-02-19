#ifndef MANUALIMPORT_H
#define MANUALIMPORT_H

#include <QWidget>

namespace Ui {
  class ManualImport;
}

class ManualImport : public QWidget
{
  Q_OBJECT

public:
  explicit ManualImport(QWidget *parent = nullptr);
  ~ManualImport();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

private:
  Ui::ManualImport *ui;
  QStringList FileNames;
  QStringList FileNameShortList;
  QStringList FileName;
};

#endif // MANUALIMPORT_H
