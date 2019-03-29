#ifndef MANUALROI_H
#define MANUALROI_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <QTimer>

namespace Ui {
  class ManualROI;
}

class ManualROI : public QDialog
{
  Q_OBJECT

public:
  explicit ManualROI(QWidget *parent = nullptr);
  ~ManualROI();

private slots:
  void on_load_clicked();
  void update_window();

  void on_save_clicked();

  void on_cancel_clicked();

private:
  Ui::ManualROI *ui;
  cv::Mat frame;
  int x1;
  int x2;
  int y1;
  int y2;
  QString FilePath;
  cv::Mat CurrentFrame;
  QTimer *timer;
};

#endif // MANUALROI_H
