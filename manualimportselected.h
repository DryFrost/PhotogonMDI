#ifndef MANUALIMPORTSELECTED_H
#define MANUALIMPORTSELECTED_H

#include <QDialog>
#include <QDir>
namespace Ui {
  class ManualImportSelected;
}

class ManualImportSelected : public QDialog
{
  Q_OBJECT

public:
  explicit ManualImportSelected(QWidget *parent = nullptr);
  QStringList FilterNames(QString dir);
  ~ManualImportSelected();

private slots:
  void on_pushButton_clicked();

private:
  Ui::ManualImportSelected *ui;
};

#endif // MANUALIMPORTSELECTED_H
