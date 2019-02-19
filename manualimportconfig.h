#ifndef MANUALIMPORTCONFIG_H
#define MANUALIMPORTCONFIG_H

#include <QDialog>
#include <QDir>
namespace Ui {
  class ManualImportConfig;
}

class ManualImportConfig : public QDialog
{
  Q_OBJECT

public:
  explicit ManualImportConfig(QWidget *parent = nullptr);
  ~ManualImportConfig();

private slots:
  void on_pushButton_8_clicked();

  void on_pushButton_clicked();

  void on_pushButton_3_clicked();

private:
  Ui::ManualImportConfig *ui;
  QString BackgroundImg;
};

#endif // MANUALIMPORTCONFIG_H
