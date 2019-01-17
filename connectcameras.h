#ifndef CONNECTCAMERAS_H
#define CONNECTCAMERAS_H

#include <QWidget>

namespace Ui {
  class connectCameras;
}

class connectCameras : public QWidget
{
  Q_OBJECT

public:
  explicit connectCameras(QWidget *parent = nullptr);
  ~connectCameras();

private slots:
  void on_pushButton_clicked();

private:
  Ui::connectCameras *ui;
};

#endif // CONNECTCAMERAS_H
