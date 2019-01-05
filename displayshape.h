#ifndef DISPLAYSHAPE_H
#define DISPLAYSHAPE_H

#include <QWidget>
#include "workershapes.h"
#include <opencv2/opencv.hpp>

namespace Ui {
  class displayShape;
}

class displayShape : public QWidget
{
  Q_OBJECT

public:
  explicit displayShape(QWidget *parent = nullptr);
  ~displayShape();

private slots:
  void on_pushButton_clicked();

public slots:
    void updateShapeFront(cv::Mat frame);
    void updateShapeTop(cv::Mat frame);
    void updateShapeSide(cv::Mat frame);

private:
  Ui::displayShape *ui;
  cv::Mat shapeFront;
  cv::Mat shapeTop;
  cv::Mat shapeSide;
  cv::Mat displayFrame;
  QString cameraSelection;
};

#endif // DISPLAYSHAPE_H
