#ifndef DISPLAYTOP_H
#define DISPLAYTOP_H

#include <QWidget>
#include <opencv2/opencv.hpp>
namespace Ui {
  class displayTop;
}

class displayTop : public QWidget
{
  Q_OBJECT

public:
  explicit displayTop(QWidget *parent = nullptr);
  ~displayTop();
public slots:
    void updateRawFrameTop(cv::Mat frame);

private:
  Ui::displayTop *ui;
};

#endif // DISPLAYTOP_H
