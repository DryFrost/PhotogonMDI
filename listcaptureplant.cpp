#include "listcaptureplant.h"
#include "ui_listcaptureplant.h"

QList<int> list2IntA(const QStringList& treatList){
  QList<int> intList;
  foreach(const QString& list,treatList)
    for(int i = 0; i < list.size();i++){
        bool isNum = false;
        int n = QString(list.at(i)).toInt(&isNum);

        if(isNum)
          intList << n;
      }
  return intList;
}

listCapturePlant::listCapturePlant(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::listCapturePlant)
{
  QSettings internal("internal.ini",QSettings::IniFormat);
  QString ProjectDirA = internal.value("ProjectDir").value<QString>();
  ProjectName = internal.value("ProjectName").value<QString>();
  QSettings setup(ProjectDirA,QSettings::IniFormat);
  ProjectDir = setup.value("ProjectDir").value<QString>();

  numTreatments = setup.value("numTreatments").value<int>();
  numRepeats = setup.value("numRepeats").value<int>();
  treatmentList = setup.value("treatmentList").value<QStringList>();
  treatmentNumList = setup.value("treatmentNumList").value<QStringList>();
  model = setup.value("model").value<QStringList>();

  numList = list2IntA(treatmentNumList);

  FtLowM = setup.value("FtLowM").value<int>();
  FtHighM = setup.value("FtHighM").value<int>();
  Fb1LM = setup.value("Fb1LM").value<int>();
  Fb1HM = setup.value("Fb1HM").value<int>();
  Fb2LM = setup.value("Fb2LM").value<int>();
  Fb2HM = setup.value("Fb2HM").value<int>();
  FblurKM = setup.value("FblurKM").value<int>();

  StLowM = setup.value("StLowM").value<int>();
  StHighM = setup.value("StHighM").value<int>();
  Sb1LM = setup.value("Sb1LM").value<int>();
  Sb1HM = setup.value("Sb1HM").value<int>();
  Sb2LM = setup.value("Sb2LM").value<int>();
  Sb2HM = setup.value("Sb2HM").value<int>();
  SblurKM = setup.value("SblurKM").value<int>();

  TtLowM = setup.value("TtLowM").value<int>();
  TtHighM = setup.value("TtHighM").value<int>();
  Tb1LM = setup.value("Tb1LM").value<int>();
  Tb1HM = setup.value("Tb1HM").value<int>();
  Tb2LM = setup.value("Tb2LM").value<int>();
  Tb2HM = setup.value("Tb2HM").value<int>();
  TblurKM = setup.value("TblurKM").value<int>();

  x1Front = setup.value("x1Front").value<int>();
  x1Side = setup.value("x1Side").value<int>();
  x1Top = setup.value("x1Top").value<int>();

  x2Front = setup.value("x2Front").value<int>();
  x2Side = setup.value("x2Side").value<int>();
  x2Top = setup.value("x2Top").value<int>();

  y1Front = setup.value("y1Front").value<int>();
  y1Side = setup.value("y1Side").value<int>();
  y1Top = setup.value("y1Top").value<int>();

  y2Front = setup.value("y2Front").value<int>();
  y2Side = setup.value("y2Side").value<int>();
  y2Top = setup.value("y2Top").value<int>();

  a = setup.value("a").value<int>();
  b = setup.value("b").value<int>();

  BluePlantThresholdFront = setup.value("BluePlantThresholdFront").value<int>();
  BluePlantBlurFront = setup.value("BluePlantBlurFront").value<int>();
  GreenPlantThresholdFront = setup.value("GreenPlantThresholdFront").value<int>();
  GreenPlantBlurFront = setup.value("GreenPlantBlurFront").value<int>();
  MaskAlphaThresholdDarkFront = setup.value("MaskAlphaThresholdDarkFront").value<int>();
  MaskAlphaThresholdLightFront = setup.value("MaskAlphaThresholdLightFront").value<int>();
  MaskBetaThresholdFront = setup.value("MaskBetaThresholdFront").value<int>();
  DifferenceDilateKernelSizeFront = setup.value("DifferenceDilateKernelSizeFront").value<int>();
  DifferenceErodeKernelSizeFront = setup.value("DifferenceErodeKernelSizeFront").value<int>();
  PotDilateKernelSizeFront = setup.value("PotDilateKernelSizeFront").value<int>();
  PotErodeKernelSizeFront = setup.value("PotErodeKernelSizeFront").value<int>();

  BluePlantThresholdSide = setup.value("BluePlantThresholdSide").value<int>();
  BluePlantBlurSide = setup.value("BluePlantBlurSide").value<int>();
  GreenPlantThresholdSide = setup.value("GreenPlantThresholdSide").value<int>();
  GreenPlantBlurSide = setup.value("GreenPlantBlurSide").value<int>();
  MaskAlphaThresholdDarkSide = setup.value("MaskAlphaThresholdDarkSide").value<int>();
  MaskAlphaThresholdLightSide = setup.value("MaskAlphaThresholdLightSide").value<int>();
  MaskBetaThresholdSide = setup.value("MaskBetaThresholdSide").value<int>();
  DifferenceDilateKernelSizeSide = setup.value("DifferenceDilateKernelSizeSide").value<int>();
  DifferenceErodeKernelSizeSide = setup.value("DifferenceErodeKernelSizeSide").value<int>();
  PotDilateKernelSizeSide = setup.value("PotDilateKernelSizeSide").value<int>();
  PotErodeKernelSizeSide = setup.value("PotErodeKernelSizeSide").value<int>();

  BluePlantThresholdTop = setup.value("BluePlantThresholdTop").value<int>();
  BluePlantBlurTop = setup.value("BluePlantBlurTop").value<int>();
  GreenPlantThresholdTop = setup.value("GreenPlantThresholdTop").value<int>();
  GreenPlantBlurTop = setup.value("GreenPlantBlurTop").value<int>();
  MaskAlphaThresholdDarkTop = setup.value("MaskAlphaThresholdDarkTop").value<int>();
  MaskAlphaThresholdLightTop = setup.value("MaskAlphaThresholdLightTop").value<int>();
  MaskBetaThresholdTop = setup.value("MaskBetaThresholdTop").value<int>();
  DifferenceDilateKernelSizeTop = setup.value("DifferenceDilateKernelSizeTop").value<int>();
  DifferenceErodeKernelSizeTop = setup.value("DifferenceErodeKernelSizeTop").value<int>();
  PotDilateKernelSizeTop = setup.value("PotDilateKernelSizeTop").value<int>();
  PotErodeKernelSizeTop = setup.value("PotErodeKernelSizeTop").value<int>();

  ColorStandardization = setup.value("Color-Standardization").value<bool>();
  Area = setup.value("Area").value<bool>();
  HullArea = setup.value("Hull-Area").value<bool>();
  Solidity = setup.value("Solidity").value<bool>();
  Perimeter = setup.value("Perimeter").value<bool>();
  Width = setup.value("Width").value<bool>();
  Height = setup.value("Height").value<bool>();
  CMX = setup.value("CMX").value<bool>();
  CMY = setup.value("CMY").value<bool>();
  HullVerticies = setup.value("Hull-Verticies").value<bool>();
  EX = setup.value("EX").value<bool>();
  EY = setup.value("EY").value<bool>();
  EMajor = setup.value("EMajor").value<bool>();
  EMinor = setup.value("EMinor").value<bool>();
  Angle = setup.value("Angle").value<bool>();
  Eccen = setup.value("Eccen").value<bool>();
  Circ = setup.value("Circ").value<bool>();
  Round = setup.value("Round").value<bool>();
  AR = setup.value("AR").value<bool>();

  ui->setupUi(this);

  QString instructions;
  instructions = "Clear the chamber";
  ui->instructions->setText(instructions);
}

listCapturePlant::~listCapturePlant()
{
  delete ui;
}

void listCapturePlant::appendMessage(const QString &text)
{
  ui->plainTextEdit->appendPlainText(text);

}

void listCapturePlant::updateRawFrameTop(cv::Mat frame){
  rawTop = frame;
}

void listCapturePlant::updateRawFrameFront(cv::Mat frame){
  rawFront = frame;

}

void listCapturePlant::updateRawFrameSide(cv::Mat frame){
  rawSide = frame;
}

void listCapturePlant::on_captureBlank_clicked()
{
  blankTop=rawTop;
  blankSide=rawSide;
  blankFront=rawFront;

  QString instructions;

  instructions = "Please Load "+treatmentList[a]+" "+model[b];
  ui->instructions->setText(instructions);
}

void listCapturePlant::on_capturePlant_clicked()
{

  accTop=rawTop;
  accSide=rawSide;
  accFront=rawFront;

  Mat disTop;
  Mat disSide;
  Mat disFront;

  cv::Size size(164,150);
  cv::resize(accTop,disTop,size);
  cv::cvtColor(disTop,disTop,cv::COLOR_BGR2RGB);
  cv::resize(accSide,disSide,size);
  cv::cvtColor(disSide,disSide,cv::COLOR_BGR2RGB);
  cv::resize(accFront,disFront,size);
  cv::cvtColor(disFront,disFront,cv::COLOR_BGR2RGB);


  QImage Front((uchar*)disFront.data,disFront.cols,disFront.rows,disFront.step,QImage::Format_RGB888);
  QImage Side((uchar*)disSide.data,disSide.cols,disSide.rows,disSide.step,QImage::Format_RGB888);
  QImage Top((uchar*)disTop.data,disTop.cols,disTop.rows,disTop.step,QImage::Format_RGB888);

  ui->label->setPixmap(QPixmap::fromImage(Front));
  ui->label_2->setPixmap(QPixmap::fromImage(Side));
  ui->label_3->setPixmap(QPixmap::fromImage(Top));


  ComputerVision cvA;

  Mat noFBG = cvA.remove_background(rawFront,blankFront,FblurKM,FtLowM,FtHighM,Fb1LM,Fb1HM,Fb2LM,Fb2HM,x1Front,y1Front,x2Front,y2Front,"Front",
                                    ColorStandardization,BluePlantThresholdFront,BluePlantBlurFront,GreenPlantThresholdFront,GreenPlantBlurFront,MaskAlphaThresholdDarkFront,MaskAlphaThresholdLightFront,
                                    MaskBetaThresholdFront,DifferenceDilateKernelSizeFront,DifferenceErodeKernelSizeFront,PotDilateKernelSizeFront,PotErodeKernelSizeFront);
  Mat noTBG = cvA.remove_background(rawTop,blankTop,TblurKM,TtLowM,TtHighM,Tb1LM,Tb1HM,Tb2LM,Tb2HM,x1Top,y1Top,x2Top,y2Top,"Top",
                                    ColorStandardization,BluePlantThresholdTop,BluePlantBlurTop,GreenPlantThresholdTop,GreenPlantBlurTop,MaskAlphaThresholdDarkTop,MaskAlphaThresholdLightTop,
                                    MaskBetaThresholdTop,DifferenceDilateKernelSizeTop,DifferenceErodeKernelSizeTop,PotDilateKernelSizeTop,PotErodeKernelSizeTop);
  Mat noSBG = cvA.remove_background(rawSide,blankSide,SblurKM,StLowM,StHighM,Sb1LM,Sb1HM,Sb2LM,Sb2HM,x1Side,y1Side,x2Side,y2Side,"Side",
                                    ColorStandardization,BluePlantThresholdSide,BluePlantBlurSide,GreenPlantThresholdSide,GreenPlantBlurSide,MaskAlphaThresholdDarkSide,MaskAlphaThresholdLightSide,
                                    MaskBetaThresholdSide,DifferenceDilateKernelSizeSide,DifferenceErodeKernelSizeSide,PotDilateKernelSizeSide,PotErodeKernelSizeSide);

  qDebug() << "Finished Removal of BG";
  std::vector<Point> ccFront = cvA.get_cc(noFBG,x1Front,y1Front,x2Front,y2Front);
  std::vector<Point> ccTop = cvA.get_cc(noTBG,x1Top,y1Top,x2Top,y2Top);
  std::vector<Point> ccSide = cvA.get_cc(noSBG,x1Side,y1Side,x2Side,y2Side);
  qDebug() << "Finished Getting CC";

  shapesTop = cvA.get_shapes(ccTop,noTBG);
  shapesFront = cvA.get_shapes(ccFront,noFBG);
  shapesSide = cvA.get_shapes(ccSide,noSBG);
  qDebug() << "Finished Capturing of Shape Info";

  Mat shapesImageTop = cvA.drawShapes(accTop,ccTop);
  Mat shapesImageFront = cvA.drawShapes(accFront,ccFront);
  Mat shapesImageSide = cvA.drawShapes(accSide,ccSide);


  HistogramImageTop = cvA.get_RGB_HIST(accTop,noTBG);
  HistogramImageFront = cvA.get_RGB_HIST(accFront,noFBG);
  HistogramImageSide = cvA.get_RGB_HIST(accSide,noSBG);

  FrontcolorR=cvA.get_color_R(accFront,noFBG);
  FrontcolorG=cvA.get_color_G(accFront,noFBG);
  FrontcolorB=cvA.get_color_B(accFront,noFBG);
  FrontcolorL=cvA.get_color_L(accFront,noFBG);
  FrontcolorGM=cvA.get_color_GM(accFront,noFBG);
  FrontcolorBY=cvA.get_color_BY(accFront,noFBG);
  FrontcolorHue=cvA.get_color_Hue(accFront,noFBG);
  FrontcolorSaturation=cvA.get_color_Saturation(accFront,noFBG);
  FrontcolorValue=cvA.get_color_Value(accFront,noFBG);

  SidecolorR=cvA.get_color_R(accSide,noSBG);
  SidecolorG=cvA.get_color_G(accSide,noSBG);
  SidecolorB=cvA.get_color_B(accSide,noSBG);
  SidecolorL=cvA.get_color_L(accSide,noSBG);
  SidecolorGM=cvA.get_color_GM(accSide,noSBG);
  SidecolorBY=cvA.get_color_BY(accSide,noSBG);
  SidecolorHue=cvA.get_color_Hue(accSide,noSBG);
  SidecolorSaturation=cvA.get_color_Saturation(accSide,noSBG);
  SidecolorValue=cvA.get_color_Value(accSide,noSBG);

  TopcolorR=cvA.get_color_R(accTop,noTBG);
  TopcolorG=cvA.get_color_G(accTop,noTBG);
  TopcolorB=cvA.get_color_B(accTop,noTBG);
  TopcolorL=cvA.get_color_L(accTop,noTBG);
  TopcolorGM=cvA.get_color_GM(accTop,noTBG);
  TopcolorBY=cvA.get_color_BY(accTop,noTBG);
  TopcolorHue=cvA.get_color_Hue(accTop,noTBG);
  TopcolorSaturation=cvA.get_color_Saturation(accTop,noTBG);
  TopcolorValue=cvA.get_color_Value(accTop,noTBG);

  //imwrite("/Users/dnguyen/Desktop/mask.png",noFBG);

  emit sendShapeTop(shapesImageTop);
  emit sendShapeFront(shapesImageFront);
  emit sendShapeSide(shapesImageSide);

  emit sendMaskTop(noTBG);
  emit sendMaskFront(noFBG);
  emit sendMaskSide(noSBG);

  emit sendHistogramTop(HistogramImageTop);
  emit sendHistogramFront(HistogramImageFront);
  emit sendHistogramSide(HistogramImageSide);

  QString instructions;

  instructions = "Click Accept if "+treatmentList[a]+" "+model[b]+" is Correct";
  ui->instructions->setText(instructions);



}

void listCapturePlant::on_accept_clicked()
{

  QString instructions;

  instructions = "Clear the chamber";
  ui->instructions->setText(instructions);
  b=b+1;
  if(b>=numList[a]){
      b=0;
      a=a+1;
    }
  if(a>=numTreatments){
      a=0;
      b=0;
    }


  QString fileDir = ProjectDir+"/Data/";
  if(!QDir(fileDir).exists()){
      QDir().mkdir(fileDir);
    }
  QString fileDirP = ProjectDir+"/Data/Pictures/";
  if(!QDir(fileDirP).exists()){
      QDir().mkdir(fileDirP);
    }

  QDateTime t = QDateTime::currentDateTime();
  QString date = t.toString("dd.MM.yyyy hh:mm:ss");
  QString date1 = t.toString("dd_MM_yyyy hh_mm_ss");


  if(QFileInfo::exists(fileDir+"shape.csv")){
      qDebug()<<"File Exists";

    }else{
      QFile data(fileDir+"shape.csv");

      if(data.open(QIODevice::ReadWrite| QIODevice::Append)){
          QTextStream out(&data);
          out <<"ID,View,Date,area,hull.area,solidity,perimeter,width,height,cmx,cmy,hull.verticies,ex,ey,emajor,eminor,angle,eccen,circ,round,ar,x,y" <<'\n';

          data.close();
        }

    }

  if(QFileInfo::exists(fileDir+"color.csv")){
      qDebug()<<"File Exists";

    }else{
      QFile data(fileDir+"color.csv");

      if(data.open(QIODevice::ReadWrite| QIODevice::Append)){
          QTextStream out(&data);
          out <<"ID,View,Date,";
          for(int i=0; i<256;i++){
              out << "R."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "G."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "B."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "L."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "GM."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "BY."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "Hue."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "Saturation."<<i<<",";
            }
          for(int i=0; i<256;i++){
              out << "Value."<<i<<",";
            }
          out << "\n";

          data.close();
        }

    }

  QFile data(fileDir+"shape.csv");

  if(data.open(QFile::WriteOnly | QIODevice::Append)){
      QTextStream out(&data);

      out <<treatmentList[a]+"_"+model[b]<<","<<"Top"<<","<<date<<","<<shapesTop[0]<<","<<shapesTop[1]<<","<<shapesTop[2]<<","<<shapesTop[3]<<","<<shapesTop[4]<<","<<shapesTop[5]<<","<<shapesTop[6]<<","<<shapesTop[7]<<","<<shapesTop[8]<<","<<shapesTop[9]<<","<<shapesTop[10]<<","<<shapesTop[11]<<","<<shapesTop[12]<<","<<shapesTop[13]<<","<<shapesTop[14]<<","<<shapesTop[15]<<","<<shapesTop[16]<<","<<shapesTop[17]<<","<<shapesTop[18]<<","<<shapesTop[19]<<'\n';
      out <<treatmentList[a]+"_"+model[b]<<","<<"Front"<<","<<date<<","<<shapesFront[0]<<","<<shapesFront[1]<<","<<shapesFront[2]<<","<<shapesFront[3]<<","<<shapesFront[4]<<","<<shapesFront[5]<<","<<shapesFront[6]<<","<<shapesFront[7]<<","<<shapesFront[8]<<","<<shapesFront[9]<<","<<shapesFront[10]<<","<<shapesFront[11]<<","<<shapesFront[12]<<","<<shapesFront[13]<<","<<shapesFront[14]<<","<<shapesFront[15]<<","<<shapesFront[16]<<","<<shapesFront[17]<<","<<shapesFront[18]<<","<<shapesFront[19]<<'\n';
      out <<treatmentList[a]+"_"+model[b]<<","<<"Side"<<","<<date<<","<<shapesSide[0]<<","<<shapesSide[1]<<","<<shapesSide[2]<<","<<shapesSide[3]<<","<<shapesSide[4]<<","<<shapesSide[5]<<","<<shapesSide[6]<<","<<shapesSide[7]<<","<<shapesSide[8]<<","<<shapesSide[9]<<","<<shapesSide[10]<<","<<shapesSide[11]<<","<<shapesSide[12]<<","<<shapesSide[13]<<","<<shapesSide[14]<<","<<shapesSide[15]<<","<<shapesSide[16]<<","<<shapesSide[17]<<","<<shapesSide[18]<<","<<shapesSide[19]<<'\n';

      data.close();

    }

  QFile dataA(fileDir+"color.csv");

  if(dataA.open(QFile::WriteOnly | QIODevice::Append)){
      QTextStream out(&dataA);
      out<<treatmentList[a]+"_"+model[b]<<','<<"Top"<<","<<date<<",";
      for(int i=0; i<256;i++){
          out << TopcolorR.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorG.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorB.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorL.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorGM.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorBY.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorHue.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorSaturation.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << TopcolorValue.at<float>(i,0) <<",";
        }
      out << "\n";

      out<<treatmentList[a]+"_"+model[b]<<','<<"Front"<<","<<date<<",";
      for(int i=0; i<256;i++){
          out << FrontcolorR.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorG.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorB.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorL.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorGM.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorBY.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorHue.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorSaturation.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << FrontcolorValue.at<float>(i,0) <<",";
        }
      out << "\n";

      out<<treatmentList[a]+"_"+model[b]<<','<<"Side"<<","<<date<<",";
      for(int i=0; i<256;i++){
          out << SidecolorR.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorG.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorB.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorL.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorGM.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorBY.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorHue.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorSaturation.at<float>(i,0) <<",";
        }
      for(int i=0; i<256;i++){
          out << SidecolorValue.at<float>(i,0) <<",";
        }
      out << "\n";

      dataA.close();

    }


  QString fileNameT = fileDirP+treatmentList[a]+"_"+model[b]+"Top"+date1+".png";
  QString fileNameF = fileDirP+treatmentList[a]+"_"+model[b]+"Front"+date1+".png";
  QString fileNameS = fileDirP+treatmentList[a]+"_"+model[b]+"Side"+date1+".png";

  std::string fileNameA = fileNameT.toUtf8().constData();
  std::string fileNameB = fileNameF.toUtf8().constData();
  std::string fileNameC = fileNameS.toUtf8().constData();

  cv::imwrite(fileNameA,rawTop);
  cv::imwrite(fileNameB,rawFront);
  cv::imwrite(fileNameC,rawSide);

  QTextCursor textCursor = ui->plainTextEdit->textCursor();
  textCursor.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor,1);
  ui->plainTextEdit->setTextCursor(textCursor);


}

