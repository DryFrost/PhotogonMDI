#ifndef EDITROI_H
#define EDITROI_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <opencv2/imgproc.hpp>
#include <QSettings>

namespace Ui {
  class editROI;
}

class editROI : public QWidget
{
  Q_OBJECT

public:
  explicit editROI(QWidget *parent = nullptr);
  ~editROI();

public slots:
    void updateRawFrameTop(cv::Mat frame);
    void updateRawFrameFront(cv::Mat frame);
    void updateRawFrameSide(cv::Mat frame);

private slots:
  void on_load_clicked();

  void on_save_clicked();

  void on_cancel_clicked();

   void update_window();

private:
  Ui::editROI *ui;

  QString cameraSelection;
  cv::Mat rawFront;
  cv::Mat rawSide;
  cv::Mat rawTop;
  QTimer *timer;
  cv::Mat currentFrame;

  int x1;
  int x2;
  int y1;
  int y2;

  QString ProjectDir;
};

#endif // EDITROI_H
