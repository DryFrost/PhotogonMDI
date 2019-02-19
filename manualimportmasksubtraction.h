#ifndef MANUALIMPORTMASKSUBTRACTION_H
#define MANUALIMPORTMASKSUBTRACTION_H

#include <QDialog>

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

private:
  Ui::ManualImportMaskSubtraction *ui;
  QString ManualBackgroundPath;
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
