#include "manualimport.h"
#include "ui_manualimport.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QSettings>
#include <manualimportselected.h>
#include <opencv2/opencv.hpp>
#include <manualimportconfig.h>
#include <manualcomputervision.h>
#include <manualcomputervisionsubtraction.h>

using namespace cv;
ManualImport::ManualImport(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ManualImport)
{
  ui->setupUi(this);
  show();

}

ManualImport::~ManualImport()
{
  delete ui;
}

void ManualImport::on_pushButton_clicked()
{

  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               "/home",
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("ProjectDirManual",QVariant::fromValue(dir));
  new ManualImportSelected();




}


void ManualImport::on_pushButton_3_clicked()
{

  ComputerVisionSub CVs;
  ComputerVisionNoSub CVnS;
  QSettings internal("internal.ini",QSettings::IniFormat);
  QString ManualSegMethod = internal.value("ManualSegMethod").value<QString>();
  QString ManualBackgroundPath = internal.value("ManualBackgroundPath").value<QString>();
  ColorStandardization = internal.value("ManualColorStandard").value<bool>();
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

  Mat Blank = imread(ManualBackgroundPath.toUtf8().constData());

  for(int i = 0; i<FileName.count(); i++){
      Mat Current = imread(FileName[i].toUtf8().constData());
      if(ManualSegMethod=="Background Subtraction"){
         NoBG = CVs.remove_backgroundSub(Current,Blank,blurKM,tLowM,tHighM,b1LM,b1HM,b2LM,b2HM,x1,y1,x2,y2,ColorStandardization,BluePlantThreshold,
                                             BluePlantBlur,GreenPlantThreshold,GreenPlantBlur,MaskAlphaThresholdDark,MaskAlphaThresholdLight,
                                             MaskBetaThreshold,DifferenceDilateKernelSize,DifferenceErodeKernelSize,PotDilateKernelSize,PotErodeKernelSize);

        }
      if(ManualSegMethod=="Binary Subtraction"){
         NoBG = CVnS.remove_backgroundNoSub(Current,b1LM,b1HM,b2LM,b2HM,x1,y1,x2,y2,ColorStandardization,BluePlantThreshold,
                                             BluePlantBlur,GreenPlantThreshold,GreenPlantBlur,MaskAlphaThresholdDark,MaskAlphaThresholdLight,
                                             MaskBetaThreshold,PotDilateKernelSize,PotErodeKernelSize);

        }

      cc = CVs.get_ccSub(NoBG,x1,y1,x2,y2);
      shapes = CVs.get_shapesSub(cc,NoBG);
      shapeImage = CVs.drawShapesSub(Current,cc);
      HistogramImage = CVs.get_RGB_HISTSub(Current,NoBG);
      colorR = CVs.get_color_RSub(Current,NoBG);
      colorG = CVs.get_color_GSub(Current,NoBG);
      colorB = CVs.get_color_BSub(Current,NoBG);
      colorL = CVs.get_color_LSub(Current,NoBG);
      colorGM = CVs.get_color_GMSub(Current,NoBG);
      colorBY = CVs.get_color_BYSub(Current,NoBG);
      colorHue = CVs.get_color_HueSub(Current,NoBG);
      colorSaturation = CVs.get_color_SaturationSub(Current,NoBG);
      colorValue = CVs.get_color_ValueSub(Current,NoBG);

      QString fileDir = DestDir+"/Data/";
      if(!QDir(fileDir).exists()){
          QDir().mkdir(fileDir);
        }

      if(QFileInfo::exists(fileDir+"shape.csv")){
          qDebug()<<"File Exists";

        }else{
          QFile data(fileDir+"shape.csv");

          if(data.open(QIODevice::ReadWrite| QIODevice::Append)){
              QTextStream out(&data);
              out <<"FileName,area,hull.area,solidity,perimeter,width,height,cmx,cmy,hull.verticies,ex,ey,emajor,eminor,angle,eccen,circ,round,ar,x,y" <<'\n';
              data.close();
            }

        }
      QFile data(fileDir+"shape.csv");

      QStringList parts = FileName[i].split("/");
      QString PicName = parts.at(parts.size()-1);

      if(data.open(QFile::WriteOnly | QIODevice::Append)){
          QTextStream out(&data);

          out << PicName <<","<<shapes[0]<<","<<shapes[1]<<","<<shapes[2]<<","<<shapes[3]<<","<<shapes[4]<<","<<shapes[5]<<","<<shapes[6]<<","<<shapes[7]<<","<<shapes[8]<<","<<shapes[9]<<","<<shapes[10]<<","<<shapes[11]<<","<<shapes[12]<<","<<shapes[13]<<","<<shapes[14]<<","<<shapes[15]<<","<<shapes[16]<<","<<shapes[17]<<","<<shapes[18]<<","<<shapes[19]<<'\n';

          data.close();

        }




    }


}

void ManualImport::on_pushButton_2_clicked()
{
  QSettings internal("internal.ini",QSettings::IniFormat);
  FileName = internal.value("ManualFileDirs").value<QStringList>();
  ui->listWidget->addItems(FileName);
}

void ManualImport::on_pushButton_4_clicked()
{
  DestDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                               "/home",
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);

}

void ManualImport::on_pushButton_5_clicked()
{
  new ManualImportConfig();
}
