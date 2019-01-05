#ifndef LISTCAPTUREPLANT_H
#define LISTCAPTUREPLANT_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <QDebug>
#include <QSettings>
#include <QtCore>
#include <computervision.h>


namespace Ui {
  class listCapturePlant;
}

class listCapturePlant : public QWidget
{
  Q_OBJECT

public:
  explicit listCapturePlant(QWidget *parent = nullptr);
  ~listCapturePlant();
  void appendMessage(const QString& text);

private slots:
  void on_captureBlank_clicked();

  void on_capturePlant_clicked();

  void on_accept_clicked();

  void updateRawFrameTop(cv::Mat frame);

  void updateRawFrameFront(cv::Mat frame);

  void updateRawFrameSide(cv::Mat frame);

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
  Ui::listCapturePlant *ui;
  cv::Mat rawFront;
  cv::Mat rawSide;
  cv::Mat rawTop;
  cv::Mat blankFront;
  cv::Mat blankSide;
  cv::Mat blankTop;
  cv::Mat accFront;
  cv::Mat accSide;
  cv::Mat accTop;

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

  std::vector<double> shapesTop;
  std::vector<double> shapesFront;
  std::vector<double> shapesSide;

  int numTreatments;
  int numRepeats;
  QStringList treatmentList;
  QStringList treatmentNumList;
  QStringList model;
  QList<int> numList;

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

  int a,b;
};

#endif // LISTCAPTUREPLANT_H
