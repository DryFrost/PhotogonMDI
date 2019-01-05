#ifndef DISPLAYMASK_H
#define DISPLAYMASK_H

#include <QWidget>
#include <opencv2/opencv.hpp>

namespace Ui {
  class displayMask;
}

class displayMask : public QWidget
{
  Q_OBJECT

public:
  explicit displayMask(QWidget *parent = nullptr);
  ~displayMask();

private slots:
  void on_pushButton_clicked();

public slots:
    void updateMaskFront(cv::Mat frame);
    void updateMaskTop(cv::Mat frame);
    void updateMaskSide(cv::Mat frame);

private:
  Ui::displayMask *ui;
  cv::Mat maskFront;
  cv::Mat maskTop;
  cv::Mat maskSide;
  cv::Mat displayFrame;
  QString cameraSelection;
};

#endif // DISPLAYMASK_H
