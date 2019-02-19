#include "manualimportconfig.h"
#include "ui_manualimportconfig.h"
#include <QSettings>
#include <QFileDialog>
ManualImportConfig::ManualImportConfig(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ManualImportConfig)
{
  ui->setupUi(this);
}

ManualImportConfig::~ManualImportConfig()
{
  delete ui;
}

void ManualImportConfig::on_pushButton_8_clicked()
{
  bool colorStandard;
  QString SegMethod = ui->comboBox->currentText();
  if(ui->checkBox->isChecked()){
      colorStandard = true;
    }
  else {
      colorStandard = false;
    }
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("ManualSegMethod",QVariant::fromValue(SegMethod));
  internal.setValue("ManualColorStandard",QVariant::fromValue(colorStandard));


}

void ManualImportConfig::on_pushButton_clicked()
{
   BackgroundImg = QFileDialog::getOpenFileName(this, tr("Files"), QDir::currentPath(), tr("*.jpg *.png"));
   QSettings internal("internal.ini",QSettings::IniFormat);
   internal.setValue("ManualBackgroundPath",QVariant::fromValue(BackgroundImg));

}

void ManualImportConfig::on_pushButton_3_clicked()
{
  int tLowM = 40;
  int tHighM = 255;
  int b1LM = 0;
  int b1HM = 120;
  int b2LM = 135;
  int b2HM = 200;
  int blurKM = 6;
  int BluePlantThreshold =130;
  int BluePlantBlur =5;
  int GreenPlantThreshold=85;
  int GreenPlantBlur=5;
  int MaskAlphaThresholdDark=115;
  int MaskAlphaThresholdLight=135;
  int MaskBetaThreshold=128;
  int DifferenceDilateKernelSize=5;
  int DifferenceErodeKernelSize=5;
  int PotDilateKernelSize=2;
  int PotErodeKernelSize=3;
  QSettings internal("internal.ini",QSettings::IniFormat);

  internal.setValue("BluePlantThresholdFront",QVariant::fromValue(BluePlantThreshold));
  internal.setValue("BluePlantBlurFront",QVariant::fromValue(BluePlantBlur));
  internal.setValue("GreenPlantThresholdFront",QVariant::fromValue(GreenPlantThreshold));
  internal.setValue("GreenPlantBlurFront",QVariant::fromValue(GreenPlantBlur));
  internal.setValue("MaskAlphaThresholdDarkFront",QVariant::fromValue(MaskAlphaThresholdDark));
  internal.setValue("MaskAlphaThresholdLightFront",QVariant::fromValue(MaskAlphaThresholdLight));
  internal.setValue("MaskBetaThresholdFront",QVariant::fromValue(MaskBetaThreshold));
  internal.setValue("DifferenceDilateKernelSizeFront",QVariant::fromValue(DifferenceDilateKernelSize));
  internal.setValue("DifferenceErodeKernelSizeFront",QVariant::fromValue(DifferenceErodeKernelSize));
  internal.setValue("PotDilateKernelSizeFront",QVariant::fromValue(PotDilateKernelSize));
  internal.setValue("PotErodeKernelSizeFront",QVariant::fromValue(PotErodeKernelSize));
  internal.setValue("FtLowM",QVariant::fromValue(tLowM));
  internal.setValue("FtHighM",QVariant::fromValue(tHighM));
  internal.setValue("Fb1LM",QVariant::fromValue(b1LM));
  internal.setValue("Fb1HM",QVariant::fromValue(b1HM));
  internal.setValue("Fb2LM",QVariant::fromValue(b2LM));
  internal.setValue("Fb2HM",QVariant::fromValue(b2HM));
  internal.setValue("FblurKM",QVariant::fromValue(blurKM));


}
