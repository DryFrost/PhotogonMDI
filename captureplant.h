#ifndef CAPTUREPLANT_H
#define CAPTUREPLANT_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "zbar.h"
#include <QDebug>
#include <QSettings>
#include <QtCore>
#include <computervision.h>
#include <QTimer>
#include <myudp.h>
#include <mytcp.h>
namespace Ui {
  class capturePlant;
}

class capturePlant : public QWidget
{
  Q_OBJECT

public:
  explicit capturePlant(QWidget *parent = nullptr);
  ~capturePlant();
  void appendMessage(const QString& text);
public slots:
    void updateRawFrameTop(cv::Mat frame);
    void updateRawFrameFront(cv::Mat frame);
    void updateRawFrameSide(cv::Mat frame);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void update_window();
signals:
    void sendShapeTop(cv::Mat frame);
    void sendShapeFront(cv::Mat frame);
    void sendShapeSide(cv::Mat frame);

    void sendHistogramTop(cv::Mat frame);
    void sendHistogramFront(cv::Mat frame);
    void sendHistogramSide(cv::Mat frame);

    void sendMaskTop(cv::Mat frame);
    void sendMaskFront(cv::Mat frame);
    void sendMaskSide(cv::Mat frame);


private:
  Ui::capturePlant *ui;
  cv::Mat rawFront;
  cv::Mat rawSide;
  cv::Mat rawTop;
  cv::Mat blankFront;
  cv::Mat blankSide;
  cv::Mat blankTop;
  cv::Mat accFront;
  cv::Mat accSide;
  cv::Mat accTop;

  bool tookBlank;
  QString qrCode;
  QString ProjectDir;
  QString ProjectName;
  QString FileProjectDir;

  int FtLowM;
  int FtHighM;
  int Fb1LM;
  int Fb1HM;
  int Fb2LM;
  int Fb2HM;
  int FblurKM;

  int TtLowM;
  int TtHighM;
  int Tb1LM;
  int Tb1HM;
  int Tb2LM;
  int Tb2HM;
  int TblurKM;

  int StLowM;
  int StHighM;
  int Sb1LM;
  int Sb1HM;
  int Sb2LM;
  int Sb2HM;
  int SblurKM;

  int x1Front;
  int y1Front;
  int x2Front;
  int y2Front;

  int x1Side;
  int y1Side;
  int x2Side;
  int y2Side;

  int x1Top;
  int y1Top;
  int x2Top;
  int y2Top;

  int BluePlantThresholdFront;
  int BluePlantBlurFront;
  int GreenPlantThresholdFront;
  int GreenPlantBlurFront;
  int MaskAlphaThresholdDarkFront;
  int MaskAlphaThresholdLightFront;
  int MaskBetaThresholdFront;
  int DifferenceDilateKernelSizeFront;
  int DifferenceErodeKernelSizeFront;
  int PotDilateKernelSizeFront;
  int PotErodeKernelSizeFront;

  int BluePlantThresholdSide;
  int BluePlantBlurSide;
  int GreenPlantThresholdSide;
  int GreenPlantBlurSide;
  int MaskAlphaThresholdDarkSide;
  int MaskAlphaThresholdLightSide;
  int MaskBetaThresholdSide;
  int DifferenceDilateKernelSizeSide;
  int DifferenceErodeKernelSizeSide;
  int PotDilateKernelSizeSide;
  int PotErodeKernelSizeSide;

  int BluePlantThresholdTop;
  int BluePlantBlurTop;
  int GreenPlantThresholdTop;
  int GreenPlantBlurTop;
  int MaskAlphaThresholdDarkTop;
  int MaskAlphaThresholdLightTop;
  int MaskBetaThresholdTop;
  int DifferenceDilateKernelSizeTop;
  int DifferenceErodeKernelSizeTop;
  int PotDilateKernelSizeTop;
  int PotErodeKernelSizeTop;

  bool ColorStandardization;
  bool Area;
  bool HullArea;
  bool Solidity;
  bool Perimeter;
  bool Width;
  bool Height;
  bool CMX;
  bool CMY;
  bool HullVerticies;
  bool EX;
  bool EY;
  bool EMajor;
  bool EMinor;
  bool Angle;
  bool Eccen;
  bool Circ;
  bool Round;
  bool AR;

  bool statTop;
  bool statFront;
  bool statSide;

  QTimer *timer;
  std::vector<double> shapesTop;
  std::vector<double> shapesFront;
  std::vector<double> shapesSide;

  Mat HistogramImageTop;
  Mat HistogramImageSide;
  Mat HistogramImageFront;

  Mat FrontcolorR;
  Mat FrontcolorG;
  Mat FrontcolorB;
  Mat FrontcolorL;
  Mat FrontcolorGM;
  Mat FrontcolorBY;
  Mat FrontcolorHue;
  Mat FrontcolorSaturation;
  Mat FrontcolorValue;

  Mat SidecolorR;
  Mat SidecolorG;
  Mat SidecolorB;
  Mat SidecolorL;
  Mat SidecolorGM;
  Mat SidecolorBY;
  Mat SidecolorHue;
  Mat SidecolorSaturation;
  Mat SidecolorValue;

  Mat TopcolorR;
  Mat TopcolorG;
  Mat TopcolorB;
  Mat TopcolorL;
  Mat TopcolorGM;
  Mat TopcolorBY;
  Mat TopcolorHue;
  Mat TopcolorSaturation;
  Mat TopcolorValue;

  MyUDP* udpCP;
  MyTCP server2;



};


#endif // CAPTUREPLANT_H
