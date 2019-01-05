#ifndef CAMERASETUP_H
#define CAMERASETUP_H

#include <QDialog>

namespace Ui {
  class cameraSetup;
}

class cameraSetup : public QDialog
{
  Q_OBJECT

public:
  explicit cameraSetup(QWidget *parent = nullptr);
  ~cameraSetup();

private:
  Ui::cameraSetup *ui;
};

#endif // CAMERASETUP_H
