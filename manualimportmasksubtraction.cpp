#include "manualimportmasksubtraction.h"
#include "ui_manualimportmasksubtraction.h"
#include <QSettings>
ManualImportMaskSubtraction::ManualImportMaskSubtraction(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ManualImportMaskSubtraction)
{
  ui->setupUi(this);
  QSettings internal("internal.ini",QSettings::IniFormat);
  ManualBackgroundPath = internal.value("FtLowM").value<QString>();
  BluePlantThreshold = internal.value("BluePlantThresholdFront").value<int>();
  BluePlantBlur = internal.value("BluePlantBlurFront").value<int>();
  GreenPlantThreshold = internal.value("GreenPlantThresholdFront").value<int>();
  GreenPlantBlur = internal.value("GreenPlantBlurFront").value<int>();
  MaskAlphaThresholdDark = internal.value("MaskAlphaThresholdDarkFront").value<int>();
  MaskAlphaThresholdLight = internal.value("MaskAlphaThresholdLightFront").value<int>();
  MaskBetaThreshold = internal.value("MaskBetaThresholdFront").value<int>();
  DifferenceDilateKernelSize = internal.value("DifferenceDilateKernelSizeFront").value<int>();
  DifferenceErodeKernelSize = internal.value("DifferenceErodeKernelSizeFront").value<int>();
  PotDilateKernelSize = internal.value("PotDilateKernelSizeFront").value<int>();
  PotErodeKernelSize = internal.value("PotErodeKernelSizeFront").value<int>();
  x1 = internal.value("x1Front").value<int>();
  x2 = internal.value("x2Front").value<int>();
  y1 = internal.value("y1Front").value<int>();
  y2 = internal.value("y2Front").value<int>();
  tLowM = internal.value("FtLowM").value<int>();
  tHighM = internal.value("FtHighM").value<int>();
  b1LM = internal.value("Fb1LM").value<int>();
  b1HM = internal.value("Fb1HM").value<int>();
  b2LM = internal.value("Fb2LM").value<int>();
  b2HM = internal.value("Fb2HM").value<int>();
  blurKM = internal.value("FblurKM").value<int>();

  ui->bYFPL->setSliderPosition(b1LM);
  ui->bYSPL->setSliderPosition(b2LM);
  ui->bYFPH->setSliderPosition(b1HM);
  ui->bYSPH->setSliderPosition(b2HM);
  ui->sTL->setSliderPosition(tLowM);
  ui->sTH->setSliderPosition(tHighM);
  ui->bKS->setSliderPosition(blurKM);

  ui->BluePlantThreshold->setSliderPosition(BluePlantThreshold);
  ui->BluePlantBlur->setSliderPosition(BluePlantBlur);
  ui->GreenPlantThreshold->setSliderPosition(GreenPlantThreshold);
  ui->GreenPlantBlur->setSliderPosition(GreenPlantBlur);
  ui->MaskAlphaThresholdDark->setSliderPosition(MaskAlphaThresholdDark);
  ui->MaskAlphaThresholdLight->setSliderPosition(MaskAlphaThresholdLight);
  ui->MaskBetaThreshold->setSliderPosition(MaskBetaThreshold);
  ui->DifferenceDilateKernelSize->setSliderPosition(DifferenceDilateKernelSize);
  ui->DifferenceErodeKernelSize->setSliderPosition(DifferenceErodeKernelSize);
  ui->PotDilateKernelSize->setSliderPosition(PotDilateKernelSize);
  ui->PotErodeKernelSize->setSliderPosition(PotErodeKernelSize);

}

ManualImportMaskSubtraction::~ManualImportMaskSubtraction()
{
  delete ui;
}

void ManualImportMaskSubtraction::on_pushButton_clicked()
{

}
