#ifndef MANUALIMPORTMASK_H
#define MANUALIMPORTMASK_H

#include <QDialog>
#include <QTimer>
#include <opencv2/opencv.hpp>
namespace Ui {
  class ManualImportMask;
}

class ManualImportMask : public QDialog
{
  Q_OBJECT

public:
  explicit ManualImportMask(QWidget *parent = nullptr);
  ~ManualImportMask();

private slots:
  void on_pushButton_clicked();
  void update_window();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

private:
  Ui::ManualImportMask *ui;
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
  int PotDilateKernelSize;
  int PotErodeKernelSize;
  int x1;
  int x2;
  int y1;
  int y2;
  int b1LM;
  int b1HM;
  int b2LM;
  int b2HM;

};

#endif // MANUALIMPORTMASK_H
