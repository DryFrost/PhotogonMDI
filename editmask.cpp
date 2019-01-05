#include "editmask.h"
#include "ui_editmask.h"

using namespace cv;
using namespace std;
editMask::editMask(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::editMask)
{
  ui->setupUi(this);
  timer = new QTimer(this);
  ui->bYFPL->setSliderPosition(0);
  ui->bYSPL->setSliderPosition(135);
  ui->bYFPH->setSliderPosition(120);
  ui->bYSPH->setSliderPosition(200);
  ui->sTL->setSliderPosition(40);
  ui->sTH->setSliderPosition(255);
  ui->bKS->setSliderPosition(6);

  QSettings internal("internal.ini",QSettings::IniFormat);
  ProjectDir = internal.value("ProjectDir").value<QString>();

}

editMask::~editMask()
{
  delete ui;
}

void editMask::updateRawFrameTop(cv::Mat frame){
  rawTop=frame;

}
void editMask::updateRawFrameSide(cv::Mat frame){
  rawSide=frame;
}
void editMask::updateRawFrameFront(cv::Mat frame){
  rawFront=frame;
}

void editMask::on_sTL_sliderReleased()
{

}

void editMask::on_sTH_sliderReleased()
{

}

void editMask::on_bKS_sliderReleased()
{

}

void editMask::on_bYFPL_sliderReleased()
{

}

void editMask::on_bYFPH_sliderReleased()
{

}

void editMask::on_bYSPL_sliderReleased()
{

}

void editMask::on_bYSPH_sliderReleased()
{

}

void editMask::on_pushButton_clicked()
{

  cameraSelection = ui->comboBox->currentText();

  if(cameraSelection=="Front"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      tLowM = setup.value("FtLowM").value<int>();
      tHighM = setup.value("FtHighM").value<int>();
      b1LM = setup.value("Fb1LM").value<int>();
      b1HM = setup.value("Fb1HM").value<int>();
      b2LM = setup.value("Fb2LM").value<int>();
      b2HM = setup.value("Fb2HM").value<int>();
      blurKM = setup.value("FblurKM").value<int>();

    }
  if(cameraSelection=="Side"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      tLowM = setup.value("StLowM").value<int>();
      tHighM = setup.value("StHighM").value<int>();
      b1LM = setup.value("Sb1LM").value<int>();
      b1HM = setup.value("Sb1HM").value<int>();
      b2LM = setup.value("Sb2LM").value<int>();
      b2HM = setup.value("Sb2HM").value<int>();
      blurKM = setup.value("SblurKM").value<int>();

    }
  if(cameraSelection=="Top"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      tLowM = setup.value("TtLowM").value<int>();
      tHighM = setup.value("TtHighM").value<int>();
      b1LM = setup.value("Tb1LM").value<int>();
      b1HM = setup.value("Tb1HM").value<int>();
      b2LM = setup.value("Tb2LM").value<int>();
      b2HM = setup.value("Tb2HM").value<int>();
      blurKM = setup.value("TblurKM").value<int>();

    }

  ui->bYFPL->setSliderPosition(b1LM);
  ui->bYSPL->setSliderPosition(b2LM);
  ui->bYFPH->setSliderPosition(b1HM);
  ui->bYSPH->setSliderPosition(b2HM);
  ui->sTL->setSliderPosition(tLowM);
  ui->sTH->setSliderPosition(tHighM);
  ui->bKS->setSliderPosition(blurKM);

  currentFrame = Mat::zeros(rawFront.size(),CV_8UC3);
  blank = Mat::zeros(rawFront.size(),CV_8UC3);
  connect(timer,SIGNAL(timeout()),this,SLOT(update_window()));
  timer->start(20);

}

void editMask::update_window(){

  tLowM = ui->sTL->value();
  tHighM = ui->sTH->value();
  b1LM = ui->bYFPL->value();
  b1HM = ui->bYFPH->value();
  b2LM = ui->bYSPL->value();
  b2HM = ui->bYSPH->value();
  blurKM = ui->bKS->value();

  Mat dest;
  absdiff(blank,currentFrame,dest);
  vector<Mat> channels(3);
  split(dest,channels);
  Mat dest_blur;
  blur(channels[1],dest_blur,Size(blurKM,blurKM));
  Mat dest_thresh;
  threshold(dest_blur,dest_thresh,tLowM,tHighM,THRESH_BINARY);
  Mat dest_dilate;
  dilate(dest_thresh,dest_dilate,Mat(), Point(-1,-1),5,1,1);
  Mat dest_erode;
  erode(dest_dilate,dest_erode,Mat(),Point(-1,-1),5,1,1);

  Mat dest_lab;
  cvtColor(dest,dest_lab,CV_BGR2Lab);
  vector<Mat> channels_lab;
  split(dest_lab,channels_lab);
  Mat pot_thresh1;
  inRange(channels_lab[2],b1LM,b1HM,pot_thresh1);
  Mat pot_thresh2;
  inRange(channels_lab[2],b2LM,b2HM,pot_thresh2);
  Mat pot_or;
  bitwise_or(pot_thresh1,pot_thresh2,pot_or);
  Mat pot_dilate;
  dilate(pot_or, pot_dilate, Mat(), Point(-1, -1), 2, 1, 1);
  Mat pot_erode;
  erode(pot_dilate,pot_erode, Mat(), Point(-1, -1), 3, 1, 1);
  Mat pot_and;
  bitwise_and(pot_erode,dest_erode,pot_and);

  Size size(300,200);


  cv::resize(pot_and,pot_and,size);
  QImage qt_imageM = QImage((const unsigned char*)(pot_and.data),pot_and.cols,pot_and.rows,QImage::Format_Indexed8);
  ui->maskM->setPixmap(QPixmap::fromImage(qt_imageM));

}

void editMask::on_pushButton_2_clicked()
{
  if(cameraSelection=="Front"){
      blank = rawFront;
    }
  if(cameraSelection=="Side"){
      blank = rawSide;
    }
  if(cameraSelection=="Top"){
      blank = rawTop;
    }

}

void editMask::on_pushButton_3_clicked()
{

  if(cameraSelection=="Front"){
      currentFrame = rawFront;
    }
  if(cameraSelection=="Side"){
      currentFrame = rawSide;
    }
  if(cameraSelection=="Top"){
      currentFrame = rawTop;
    }
}

void editMask::on_pushButton_4_clicked()
{

  if(cameraSelection=="Front"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      setup.setValue("FtLowM",QVariant::fromValue(tLowM));
      setup.setValue("FtHighM",QVariant::fromValue(tHighM));
      setup.setValue("Fb1LM",QVariant::fromValue(b1LM));
      setup.setValue("Fb1HM",QVariant::fromValue(b1HM));
      setup.setValue("Fb2LM",QVariant::fromValue(b2LM));
      setup.setValue("Fb2HM",QVariant::fromValue(b2HM));
      setup.setValue("FblurKM",QVariant::fromValue(blurKM));
    }
  if(cameraSelection=="Side"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      setup.setValue("StLowM",QVariant::fromValue(tLowM));
      setup.setValue("StHighM",QVariant::fromValue(tHighM));
      setup.setValue("Sb1LM",QVariant::fromValue(b1LM));
      setup.setValue("Sb1HM",QVariant::fromValue(b1HM));
      setup.setValue("Sb2LM",QVariant::fromValue(b2LM));
      setup.setValue("Sb2HM",QVariant::fromValue(b2HM));
      setup.setValue("SblurKM",QVariant::fromValue(blurKM));
    }
  if(cameraSelection=="Top"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      setup.setValue("TtLowM",QVariant::fromValue(tLowM));
      setup.setValue("TtHighM",QVariant::fromValue(tHighM));
      setup.setValue("Tb1LM",QVariant::fromValue(b1LM));
      setup.setValue("Tb1HM",QVariant::fromValue(b1HM));
      setup.setValue("Tb2LM",QVariant::fromValue(b2LM));
      setup.setValue("Tb2HM",QVariant::fromValue(b2HM));
      setup.setValue("TblurKM",QVariant::fromValue(blurKM));
    }



}

void editMask::on_pushButton_5_clicked()
{
    close();
}
