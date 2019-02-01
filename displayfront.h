#ifndef DISPLAYFRONT_H
#define DISPLAYFRONT_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include "workerdisplayfront.h"
#include <QThread>
#define MAX_NUM_CAM1 1

namespace Ui {
  class displayFront;
}

class displayFront : public QWidget
{
  Q_OBJECT

public:
  explicit displayFront(QWidget *parent = nullptr);
  ~displayFront();

public slots:
    void updateRawFrameFront(cv::Mat frame);
    void displayframe(cv::Mat);

private:
  Ui::displayFront *ui;
  cv::Mat rawFrame;
  QThread* threads[MAX_NUM_CAM1];
  WorkerDisplayFront* workers[MAX_NUM_CAM1];

};

#endif // DISPLAYFRONT_H
