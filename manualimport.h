#ifndef MANUALIMPORT_H
#define MANUALIMPORT_H

#include <QWidget>
#include <opencv2/opencv.hpp>


namespace Ui {
  class ManualImport;
}

class ManualImport : public QWidget
{
  Q_OBJECT

public:
  explicit ManualImport(QWidget *parent = nullptr);
  ~ManualImport();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_3_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

private:
  Ui::ManualImport *ui;
  QStringList FileNames;
  QStringList FileNameShortList;
  QStringList FileName;
  int blurKM;
  int tLowM;
  int tHighM;
  int b1LM;
  int b1HM;
  int b2LM;
  int b2HM;
  int x1;
  int y1;
  int x2;
  int y2;
  bool ColorStandardization;
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
  std::vector<double> shapes;
  std::vector<cv::Point> cc;
  cv::Mat NoBG;
  cv::Mat shapeImage;
  cv::Mat HistogramImage;
  cv::Mat colorR;
  cv::Mat colorG;
  cv::Mat colorB;
  cv::Mat colorL;
  cv::Mat colorGM;
  cv::Mat colorBY;
  cv::Mat colorHue;
  cv::Mat colorSaturation;
  cv::Mat colorValue;
  QString DestDir;

};

#endif // MANUALIMPORT_H
