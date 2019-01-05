#ifndef DISPLAYFRONT_H
#define DISPLAYFRONT_H

#include <QWidget>
#include <opencv2/opencv.hpp>

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

private:
  Ui::displayFront *ui;
  cv::Mat rawFrame;
};

#endif // DISPLAYFRONT_H
