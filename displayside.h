#ifndef DISPLAYSIDE_H
#define DISPLAYSIDE_H

#include <QWidget>
#include <opencv2/opencv.hpp>
namespace Ui {
  class displaySide;
}

class displaySide : public QWidget
{
  Q_OBJECT

public:
  explicit displaySide(QWidget *parent = nullptr);
  ~displaySide();
public slots:
    void updateRawFrameSide(cv::Mat frame);

private:
  Ui::displaySide *ui;
};

#endif // DISPLAYSIDE_H
