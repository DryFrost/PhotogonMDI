#include "selectcolorchips.h"
#include "ui_selectcolorchips.h"
#include <QSettings>
#include "colorchipsmask.h"
#include "createtargetcolor.h"

using namespace cv;

selectColorChips::selectColorChips(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::selectColorChips)
{
  ui->setupUi(this);
}

selectColorChips::~selectColorChips()
{
  delete ui;
}

void selectColorChips::updateMaskTop(cv::Mat frame){
  rawTop = frame;

}

void selectColorChips::updateMaskFront(cv::Mat frame){
  rawFront = frame;
}

void selectColorChips::updateMaskSide(cv::Mat frame){
  rawSide = frame;
}

void selectColorChips::on_pushButton_clicked()
{
  cameraSelection = ui->comboBox->currentText();

  displayFrame = Mat::zeros(rawFront.size(),CV_8UC3);
  m = Mat::zeros(rawFront.size(),CV_8UC3);

}


void selectColorChips::on_pushButton_2_clicked()
{

  detectColorChips dCC;

  IplImage* sendFrame;

  if(cameraSelection =="Front"){
      displayFrame = rawFront;
      //cv::cvtColor(displayFrame,displayFrame,cv::COLOR_BGR2RGB);
      sendFrame = cvCreateImage(cvSize(displayFrame.cols,displayFrame.rows),8,3);
      IplImage ipltemp=displayFrame;
      cvCopy(&ipltemp,sendFrame);

      try{
        IplImage *out = dCC.find_macbeth(sendFrame);
        m = cv::cvarrToMat(out);
      }
      catch(cv::Exception& e)
      {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
      }

    }
  if(cameraSelection == "Side"){
      displayFrame  = rawSide;
      displayFrame = rawFront;
      cv::cvtColor(displayFrame,displayFrame,cv::COLOR_BGR2RGB);
      sendFrame = cvCreateImage(cvSize(displayFrame.cols,displayFrame.rows),8,3);
      IplImage ipltemp=displayFrame;
      cvCopy(&ipltemp,sendFrame);
      IplImage *out = dCC.find_macbeth(sendFrame);
      cv::Mat m = cv::cvarrToMat(out);

    }
  if(cameraSelection == "Top"){
      displayFrame = rawTop;
      displayFrame = rawFront;
      cv::cvtColor(displayFrame,displayFrame,cv::COLOR_BGR2RGB);
      sendFrame = cvCreateImage(cvSize(displayFrame.cols,displayFrame.rows),8,3);
      IplImage ipltemp=displayFrame;
      cvCopy(&ipltemp,sendFrame);
      IplImage *out = dCC.find_macbeth(sendFrame);
      cv::Mat m = cv::cvarrToMat(out);
    }


  QSettings internal("internal.ini",QSettings::IniFormat);
  ColorInfo = internal.value("ColorInfo").value<QStringList>();

  ui->tableWidget->setRowCount(22);
  ui->tableWidget->setColumnCount(3);
  QStringList headerInfo;
  headerInfo<<"B"<<"G"<<"R";
  ui->tableWidget->setHorizontalHeaderLabels(headerInfo);

  int a=0;
  int b=0;

  for(int i = 0; i < ColorInfo.size(); i++){
      ui->tableWidget->setItem(a,b,new QTableWidgetItem(ColorInfo[i]));
      b=b+1;
      if(b==3){
          b=0;
          a=a+1;
      }
  }

  Size size(400,225);
  cv::resize(m,m,size);
  cvtColor(m,m,CV_BGR2RGB);
  QImage qt_image = QImage((const unsigned char*)(m.data),m.cols,m.rows,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qt_image));
}

void selectColorChips::on_pushButton_3_clicked()
{
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("CurrentView",QVariant::fromValue(cameraSelection));
  colorChipsMask cCM;

  createTargetColor cTC;

  IplImage* sendFrame;

  if(cameraSelection =="Front"){
      displayFrame = rawFront;
      //cv::cvtColor(displayFrame,displayFrame,cv::COLOR_BGR2RGB);
      sendFrame = cvCreateImage(cvSize(displayFrame.cols,displayFrame.rows),8,3);
      IplImage ipltemp=displayFrame;
      cvCopy(&ipltemp,sendFrame);

      try{
        cCM.find_macbeth1(sendFrame);
        cTC.ColorFound(rawFront);
      }
      catch(cv::Exception& e)
      {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
      }

    }
  if(cameraSelection == "Side"){
      displayFrame  = rawSide;
      displayFrame = rawFront;
      cv::cvtColor(displayFrame,displayFrame,cv::COLOR_BGR2RGB);
      sendFrame = cvCreateImage(cvSize(displayFrame.cols,displayFrame.rows),8,3);
      IplImage ipltemp=displayFrame;
      cvCopy(&ipltemp,sendFrame);
      try{
        cCM.find_macbeth1(sendFrame);
        cTC.ColorFound(rawSide);
      }
      catch(cv::Exception& e)
      {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
      }


    }
  if(cameraSelection == "Top"){
      displayFrame = rawTop;
      displayFrame = rawFront;
      cv::cvtColor(displayFrame,displayFrame,cv::COLOR_BGR2RGB);
      sendFrame = cvCreateImage(cvSize(displayFrame.cols,displayFrame.rows),8,3);
      IplImage ipltemp=displayFrame;
      cvCopy(&ipltemp,sendFrame);
      try{
        cCM.find_macbeth1(sendFrame);
        cTC.ColorFound(rawTop);
      }
      catch(cv::Exception& e)
      {
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
      }

    }

}
