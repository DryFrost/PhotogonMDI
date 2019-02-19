#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setCentralWidget(ui->mdiArea);
  qRegisterMetaType<cv::Mat >("cv::Mat");

  mInternal = new QSettings("internal.ini",QSettings::IniFormat);

  mMenuRecentFiles = new QMenu(this);
  for (int n=0; n<maxRecentFiles;++n){
      mActionRecentFiles[n] = new QAction(this);
      mActionRecentFiles[n]->setVisible(false);
      connect(mActionRecentFiles[n],&QAction::triggered,this,&MainWindow::openRecentFile);
      ui->menuFile->addAction(mActionRecentFiles[n]);
    }
  mActionSeparatorRecentFiles = ui->menuFile->addSeparator();
  mActionSeparatorRecentFiles->setVisible(false);
  updateRecentFilesMenu();

  new welcomeScreen();
}

void MainWindow::openRecentFile(){
  loadFile(qobject_cast<QAction*>(sender())->data().toString());
  ui->actionActivate_Cameras->setEnabled(true);
}

void MainWindow::loadFile(QString filePath){
  if(!filePath.isEmpty()){
      //load script
      mProject = new QSettings(filePath,QSettings::IniFormat);
      mInternal->setValue("ProjectDir",filePath);
      qDebug()<<filePath;
      Qr = mProject->value("Qr").toString();
      List = mProject->value("List").toString();
      Manual = mProject->value("Manual").toString();
      ColorStandardization = mProject->value("Color-Standardization").toBool();
      setRecentFile(filePath);
    }
}

void MainWindow::setRecentFile(QString filePath){
  QStringList files = mInternal->value("recentFiles").toStringList();
  files.removeAll(filePath);
  files.prepend(filePath);
  while (files.size()>maxRecentFiles)
    files.removeLast();
  mInternal->setValue("recentFiles",files);
  updateRecentFilesMenu();
}

void MainWindow::removeRecentFile(QString filePath){
  QStringList files = mInternal->value("recentFiles").toStringList();
  files.removeAll(filePath);
  mInternal->setValue("recentFiles",files);
  updateRecentFilesMenu();
}

void MainWindow::updateRecentFilesMenu(){
  QStringList files = mInternal->value("recentFiles").toStringList();
  int numRecentFiles;
  if (files.size() < maxRecentFiles)
    numRecentFiles = files.size();
  else numRecentFiles = maxRecentFiles;
  mActionSeparatorRecentFiles->setVisible(numRecentFiles>0);
  mMenuRecentFiles->clear();
  for(int n=0; n<numRecentFiles;++n){
      mActionRecentFiles[n]->setText(tr("&%1%2").arg(n+1).arg(QFileInfo(files[n]).fileName()));
      mActionRecentFiles[n]->setData(files[n]);
      mActionRecentFiles[n]->setVisible(true);
      mMenuRecentFiles->addAction(mActionRecentFiles[n]);
    }
  for(int n=numRecentFiles;n<maxRecentFiles;++n)
    mActionRecentFiles[n]->setVisible(false);
}

void MainWindow::on_actionOpen_Project_triggered()
{

  loadFile(QFileDialog::getOpenFileName(this,
                                        tr("Open Existing Project"), "",
                                        tr("Project File (*.ini);;All Files (*)")));

  ui->actionActivate_Cameras->setEnabled(true);

}

void MainWindow::on_actionNew_Project_triggered()
{
  new NewProject();
  //ui->actionActivate_Cameras->setEnabled(true);
}


void MainWindow::displayFrame(cv::Mat frame, int  index)
{
  //CREATE EMPTY FRAMES;

  if(index == 0){
      RawFrameFront=frame;

    }
  if(index == 1){
      RawFrameSide=frame;

    }
  if(index == 2){
      RawFrameTop=frame;
    }
  emit SendRawFrameTop(RawFrameTop);
  emit SendRawFrameSide(RawFrameSide);
  emit SendRawFrameFront(RawFrameFront);


}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_actionOpen_Camera_triggered()
{
  ui->actionDisplay_Masks->setEnabled(true);
  ui->actionDisplay_Shapes->setEnabled(true);
  ui->actionDisplay_Histograms->setEnabled(true);
  qDebug() << Qr;
  if(Qr=="true"){
      qDebug() << "trying to open QR";
      mCapturePlant = new capturePlant(this);
      loadSubWindow(mCapturePlant);

      //if(mCapturePlant){
          connect(this,SIGNAL(SendRawFrameTop(cv::Mat)),mCapturePlant,SLOT(updateRawFrameTop(cv::Mat)));
          connect(this,SIGNAL(SendRawFrameFront(cv::Mat)),mCapturePlant,SLOT(updateRawFrameFront(cv::Mat)));
          connect(this,SIGNAL(SendRawFrameSide(cv::Mat)),mCapturePlant,SLOT(updateRawFrameSide(cv::Mat)));
        //}
    }
  if(List=="true"){
      qDebug() << "trying to open List";
      mListCapturePlant = new listCapturePlant(this);
      loadSubWindow(mListCapturePlant);

      //if(mListCapturePlant){
          connect(this,SIGNAL(SendRawFrameTop(cv::Mat)),mListCapturePlant,SLOT(updateRawFrameTop(cv::Mat)));
          connect(this,SIGNAL(SendRawFrameFront(cv::Mat)),mListCapturePlant,SLOT(updateRawFrameFront(cv::Mat)));
          connect(this,SIGNAL(SendRawFrameSide(cv::Mat)),mListCapturePlant,SLOT(updateRawFrameSide(cv::Mat)));
        //}
    }

}

void MainWindow::loadSubWindow(QWidget *widget)
{
  auto window = ui->mdiArea->addSubWindow(widget);
  window->setWindowTitle(widget->windowTitle());
  window->show();
}

void MainWindow::on_actionDisplay_Front_Camera_triggered()
{
  mDisplayFront = new displayFront(this);
  loadSubWindow(mDisplayFront);
  if(mDisplayFront){
      connect(this,SIGNAL(SendRawFrameFront(cv::Mat)),mDisplayFront,SLOT(updateRawFrameFront(cv::Mat)));
    }

}

void MainWindow::on_actionDisplay_Side_Camera_triggered()
{

  mDisplaySide = new displaySide(this);
  loadSubWindow(mDisplaySide);
  if(mDisplaySide){
      connect(this,SIGNAL(SendRawFrameSide(cv::Mat)),mDisplaySide,SLOT(updateRawFrameSide(cv::Mat)));
    }

}

void MainWindow::on_actionDisplay_Top_Camera_triggered()
{

  mDisplayTop = new displayTop(this);
  loadSubWindow(mDisplayTop);
  if(mDisplayTop){
      connect(this,SIGNAL(SendRawFrameTop(cv::Mat)),mDisplayTop,SLOT(updateRawFrameTop(cv::Mat)));
    }

}

void MainWindow::on_actionModify_Region_of_Intrest_triggered()
{
  mEditROI = new editROI(this);
  loadSubWindow(mEditROI);
  if(mEditROI){
      connect(this,SIGNAL(SendRawFrameTop(cv::Mat)),mEditROI,SLOT(updateRawFrameTop(cv::Mat)));
      connect(this,SIGNAL(SendRawFrameSide(cv::Mat)),mEditROI,SLOT(updateRawFrameSide(cv::Mat)));
      connect(this,SIGNAL(SendRawFrameFront(cv::Mat)),mEditROI,SLOT(updateRawFrameFront(cv::Mat)));
    }

}

void MainWindow::on_actionModify_Mask_Properties_triggered()
{
  mEditMask = new editMask(this);
  loadSubWindow(mEditMask);
  if(mEditMask){
      connect(this,SIGNAL(SendRawFrameTop(cv::Mat)),mEditMask,SLOT(updateRawFrameTop(cv::Mat)));
      connect(this,SIGNAL(SendRawFrameSide(cv::Mat)),mEditMask,SLOT(updateRawFrameSide(cv::Mat)));
      connect(this,SIGNAL(SendRawFrameFront(cv::Mat)),mEditMask,SLOT(updateRawFrameFront(cv::Mat)));
    }
}

void MainWindow::on_actionActivate_Cameras_triggered()
{

  udp = new MyUDP(this);

  QString command = "Capture";
  udp->deviceDiscover(command);

  QString FrontCameraIP = mInternal->value("FrontCameraIP").toString();
  QString SideCameraIP = mInternal->value("SideCameraIP").toString();
  QString TopCameraIP = mInternal->value("TopCameraIP").toString();

  QStringList ipList;
  ipList<<FrontCameraIP<<SideCameraIP<<TopCameraIP;

  int numCams = 1;
  for(int i = 0; i < numCams; i++)
  {
      threads[i] = new QThread;
      workers[i] = new Worker(QString(ipList[i]),i);

      workers[i]->moveToThread(threads[i]);

      connect(workers[i], SIGNAL(frameFinished(cv::Mat, int)), this, SLOT(displayFrame(cv::Mat,int)));
      connect(threads[i], SIGNAL(started()), workers[i], SLOT(readVideo()));

      connect(workers[i], SIGNAL(finished(int)), threads[i], SLOT(quit()));
      connect(workers[i], SIGNAL(finished(int)), workers[i], SLOT(deleteLater()));

      connect(threads[i], SIGNAL(finished()), threads[i], SLOT(deleteLater()));

      threads[i]->start();
  }

  ui->actionDisplay_Top_Camera->setEnabled(true);
  ui->actionDisplay_Side_Camera->setEnabled(true);
  ui->actionDisplay_Front_Camera->setEnabled(true);
  ui->actionModify_Mask_Properties->setEnabled(true);
  ui->actionModify_Region_of_Intrest->setEnabled(true);
  if(!ColorStandardization){
      ui->actionOpen_Camera->setEnabled(true);
    }


  ui->actionDetect_Color_Chips->setEnabled(true);

}

void MainWindow::on_actionDisplay_Shapes_triggered()
{
  if(Qr=="true"){
      mDisplayShape = new displayShape(this);
      loadSubWindow(mDisplayShape);
      if(mDisplayShape){
          connect(mCapturePlant,SIGNAL(sendShapeTop(cv::Mat)),mDisplayShape,SLOT(updateShapeTop(cv::Mat)));
          connect(mCapturePlant,SIGNAL(sendShapeFront(cv::Mat)),mDisplayShape,SLOT(updateShapeFront(cv::Mat)));
          connect(mCapturePlant,SIGNAL(sendShapeSide(cv::Mat)),mDisplayShape,SLOT(updateShapeSide(cv::Mat)));
        }
    }
  if(List=="true"){
      mDisplayShape = new displayShape(this);
      loadSubWindow(mDisplayShape);
      if(mDisplayShape){
          connect(mListCapturePlant,SIGNAL(sendShapeTop(cv::Mat)),mDisplayShape,SLOT(updateShapeTop(cv::Mat)));
          connect(mListCapturePlant,SIGNAL(sendShapeFront(cv::Mat)),mDisplayShape,SLOT(updateShapeFront(cv::Mat)));
          connect(mListCapturePlant,SIGNAL(sendShapeSide(cv::Mat)),mDisplayShape,SLOT(updateShapeSide(cv::Mat)));
        }
    }
}

void MainWindow::on_actionDisplay_Masks_triggered()
{

  if(Qr=="true"){
      mDisplayMask = new displayMask(this);
      loadSubWindow(mDisplayMask);
      if(mDisplayMask){
          connect(mCapturePlant,SIGNAL(sendMaskTop(cv::Mat)),mDisplayMask,SLOT(updateMaskTop(cv::Mat)));
          connect(mCapturePlant,SIGNAL(sendMaskFront(cv::Mat)),mDisplayMask,SLOT(updateMaskFront(cv::Mat)));
          connect(mCapturePlant,SIGNAL(sendMaskSide(cv::Mat)),mDisplayMask,SLOT(updateMaskSide(cv::Mat)));
        }
    }
  if(List=="true"){
      mDisplayMask = new displayMask(this);
      loadSubWindow(mDisplayMask);
      if(mDisplayMask){
          connect(mListCapturePlant,SIGNAL(sendMaskTop(cv::Mat)),mDisplayMask,SLOT(updateMaskTop(cv::Mat)));
          connect(mListCapturePlant,SIGNAL(sendMaskFront(cv::Mat)),mDisplayMask,SLOT(updateMaskFront(cv::Mat)));
          connect(mListCapturePlant,SIGNAL(sendMaskSide(cv::Mat)),mDisplayMask,SLOT(updateMaskSide(cv::Mat)));
        }
    }

}

void MainWindow::on_actionDisplay_Histograms_triggered()
{
  if(Qr=="true"){
      mDisplayHistogram = new displayHistogram(this);
      loadSubWindow(mDisplayHistogram);
      if(mDisplayHistogram){
          connect(mCapturePlant,SIGNAL(sendHistogramTop(cv::Mat)),mDisplayHistogram,SLOT(updateHistogramTop(cv::Mat)));
          connect(mCapturePlant,SIGNAL(sendHistogramFront(cv::Mat)),mDisplayHistogram,SLOT(updateHistogramFront(cv::Mat)));
          connect(mCapturePlant,SIGNAL(sendHistogramSide(cv::Mat)),mDisplayHistogram,SLOT(updateHistogramSide(cv::Mat)));
        }
    }
  if(List=="true"){
      mDisplayHistogram = new displayHistogram(this);
      loadSubWindow(mDisplayHistogram);
      if(mDisplayShape){
          connect(mListCapturePlant,SIGNAL(sendHistogramTop(cv::Mat)),mDisplayHistogram,SLOT(updateHistogramTop(cv::Mat)));
          connect(mListCapturePlant,SIGNAL(sendHistogramFront(cv::Mat)),mDisplayHistogram,SLOT(updateHistogramFront(cv::Mat)));
          connect(mListCapturePlant,SIGNAL(sendHistogramSide(cv::Mat)),mDisplayHistogram,SLOT(updateHistogramSide(cv::Mat)));
        }
    }
}

void MainWindow::on_actionDetect_Color_Chips_triggered()
{
  mSelectColorChips = new selectColorChips(this);
  loadSubWindow(mSelectColorChips);
  if(mSelectColorChips){
      connect(this,SIGNAL(SendRawFrameTop(cv::Mat)),mSelectColorChips,SLOT(updateMaskTop(cv::Mat)));
      connect(this,SIGNAL(SendRawFrameSide(cv::Mat)),mSelectColorChips,SLOT(updateMaskSide(cv::Mat)));
      connect(this,SIGNAL(SendRawFrameFront(cv::Mat)),mSelectColorChips,SLOT(updateMaskFront(cv::Mat)));
    }
  ui->actionOpen_Camera->setEnabled(true);
}

void MainWindow::on_actionDetect_Cameras_triggered()
{
    new cameraDiscovery();
}

void MainWindow::on_actionLoad_Cameras_triggered()
{
  mConnectCameras = new connectCameras(this);
  loadSubWindow(mConnectCameras);
}

void MainWindow::on_actionImport_Images_triggered()
{
  mManualImport = new ManualImport(this);
  loadSubWindow(mManualImport);
}
