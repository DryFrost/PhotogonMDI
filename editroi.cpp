#include "editroi.h"
#include "ui_editroi.h"

using namespace cv;

editROI::editROI(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::editROI)
{
  ui->setupUi(this);
  timer = new QTimer(this);

  QSettings internal("internal.ini",QSettings::IniFormat);
  ProjectDir = internal.value("ProjectDir").value<QString>();
}

void editROI::updateRawFrameTop(cv::Mat frame){
  rawTop=frame;

}
void editROI::updateRawFrameSide(cv::Mat frame){
  rawSide=frame;
}
void editROI::updateRawFrameFront(cv::Mat frame){
  rawFront=frame;
}

editROI::~editROI()
{
  delete ui;
}

void editROI::on_load_clicked()
{

  cameraSelection = ui->comboBox->currentText();

  if(cameraSelection=="Front"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      x1 = setup.value("x1Front").value<int>();
      y1 = setup.value("y1Front").value<int>();
      x2 = setup.value("x2Front").value<int>();
      y2 = setup.value("y2Front").value<int>();


    }
  if(cameraSelection=="Side"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      x1 = setup.value("x1Side").value<int>();
      y1 = setup.value("y1Side").value<int>();
      x2 = setup.value("x2Side").value<int>();
      y2 = setup.value("y2Side").value<int>();

    }
  if(cameraSelection=="Top"){
      QSettings setup(ProjectDir,QSettings::IniFormat);
      x1 = setup.value("x1Top").value<int>();
      y1 = setup.value("y1Top").value<int>();
      x2 = setup.value("x2Top").value<int>();
      y2 = setup.value("y2Top").value<int>();

    }

  ui->x1->setValue(x1);
  ui->x2->setValue(x2);
  ui->y1->setValue(y1);
  ui->y2->setValue(y2);

  currentFrame = Mat::zeros(rawFront.size(),CV_8UC3);
  connect(timer,SIGNAL(timeout()),this,SLOT(update_window()));
  timer->start(20);

}

void editROI::on_save_clicked()
{

  if(cameraSelection=="Front"){

      QSettings setup(ProjectDir,QSettings::IniFormat);
      setup.setValue("x1Front",QVariant::fromValue(x1));
      setup.setValue("y1Front",QVariant::fromValue(y1));
      setup.setValue("x2Front",QVariant::fromValue(x2));
      setup.setValue("y2Front",QVariant::fromValue(y2));

    }
  if(cameraSelection =="Side"){

      QSettings setup(ProjectDir,QSettings::IniFormat);
      setup.setValue("x1Side",QVariant::fromValue(x1));
      setup.setValue("y1Side",QVariant::fromValue(y1));
      setup.setValue("x2Side",QVariant::fromValue(x2));
      setup.setValue("y2Side",QVariant::fromValue(y2));

    }
  if(cameraSelection == "Top"){

      QSettings setup(ProjectDir,QSettings::IniFormat);
      setup.setValue("x1Top",QVariant::fromValue(x1));
      setup.setValue("y1Top",QVariant::fromValue(y1));
      setup.setValue("x2Top",QVariant::fromValue(x2));
      setup.setValue("y2Top",QVariant::fromValue(y2));

    }

}

void editROI::on_cancel_clicked()
{

}

void editROI::update_window(){

  if(cameraSelection=="Front"){
      currentFrame=rawFront;
    }
  if(cameraSelection=="Side"){
      currentFrame=rawSide;
    }
  if(cameraSelection=="Top"){
      currentFrame=rawTop;
    }

  x1 = ui->x1->value();
  x2 = ui->x2->value();
  y1 = ui->y1->value();
  y2 = ui->y2->value();

  cv::rectangle(currentFrame,Point(x1,y1),Point(x2,y2),Scalar(255,255,0),4,LINE_8,0);

  Size size(400,225);
  cv::resize(currentFrame,currentFrame,size);
  cvtColor(currentFrame,currentFrame,CV_BGR2RGB);
  QImage qt_image = QImage((const unsigned char*)(currentFrame.data),currentFrame.cols,currentFrame.rows,QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qt_image));

}
