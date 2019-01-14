#ifndef SELECTCOLORCHIPS_H
#define SELECTCOLORCHIPS_H

#include <QWidget>
#include <detectcolorchips.h>

#include <QTimer>

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

namespace Ui {
  class selectColorChips;
}

class selectColorChips : public QWidget
{
  Q_OBJECT

public:
  explicit selectColorChips(QWidget *parent = nullptr);
  ~selectColorChips();

public slots:
    void updateMaskFront(cv::Mat frame);
    void updateMaskTop(cv::Mat frame);
    void updateMaskSide(cv::Mat frame);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
  Ui::selectColorChips *ui;
  cv::Mat displayFrame;
  cv::Mat rawFront;
  cv::Mat rawSide;
  cv::Mat rawTop;
  QString cameraSelection;
  cv::Mat m;
  QStringList ColorInfo;

};

#endif // SELECTCOLORCHIPS_H
