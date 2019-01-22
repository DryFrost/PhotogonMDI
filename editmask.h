#ifndef EDITMASK_H
#define EDITMASK_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <opencv2/imgproc.hpp>
#include <QSettings>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <utility>
#include <cmath>
#include <Eigen/Dense>
#include <QSettings>
#include <QFile>

namespace Ui {
  class editMask;
}

class editMask : public QWidget
{
  Q_OBJECT

public:
  explicit editMask(QWidget *parent = nullptr);
  ~editMask();

public slots:
    void updateRawFrameTop(cv::Mat frame);
    void updateRawFrameFront(cv::Mat frame);
    void updateRawFrameSide(cv::Mat frame);

private slots:
  void on_sTL_sliderReleased();

  void on_sTH_sliderReleased();

  void on_bKS_sliderReleased();

  void on_bYFPL_sliderReleased();

  void on_bYFPH_sliderReleased();

  void on_bYSPL_sliderReleased();

  void on_bYSPH_sliderReleased();

  void on_pushButton_clicked();

  void update_window();

  void on_pushButton_2_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

private:
  Ui::editMask *ui;
  QString cameraSelection;
  cv::Mat rawFront;
  cv::Mat rawSide;
  cv::Mat rawTop;
  QTimer *timer;
  cv::Mat currentFrame;
  cv::Mat blank;

  int tLowM;
  int tHighM;
  int b1LM;
  int b1HM;
  int b2LM;
  int b2HM;
  int blurKM;

  int BluePlantThreshold;
  int BluePlantBlur;
  int GreenPlantThreshold;
  int GreenPlantBlur;
  int MaskAlphaThresholdDark;
  int MaskAlphaThresholdLight;
  int MaskBetaThreshold;
  int DifferenceDilateKernelSize;
  int DifferenceErodeKernelSize;
  int PotDilateKernelSize;
  int PotErodeKernelSize;

  int x1;
  int y1;
  int x2;
  int y2;


  QString ProjectDir;
  QString ProjectName;

};

#endif // EDITMASK_H
