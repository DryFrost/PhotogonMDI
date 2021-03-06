#ifndef MANUALIMPORTMASKSUBTRACTION_H
#define MANUALIMPORTMASKSUBTRACTION_H

#include <QDialog>
#include <QTimer>
#include <opencv2/opencv.hpp>
namespace Ui {
  class ManualImportMaskSubtraction;
}

class ManualImportMaskSubtraction : public QDialog
{
  Q_OBJECT

public:
  explicit ManualImportMaskSubtraction(QWidget *parent = nullptr);
  ~ManualImportMaskSubtraction();

private slots:
  void on_pushButton_clicked();
  void update_window();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

private:
  Ui::ManualImportMaskSubtraction *ui;
  QString ManualBackgroundPath;
  QTimer *timer;
  QString PlantPic;
  cv::Mat currentFrame;
  cv::Mat blank;
  bool ManualColorStandard;
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
  int x2;
  int y1;
  int y2;
  int tLowM;
  int tHighM;
  int b1LM;
  int b1HM;
  int b2LM;
  int b2HM;
  int blurKM;

};

#endif // MANUALIMPORTMASKSUBTRACTION_H
