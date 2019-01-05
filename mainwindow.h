#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <opencv2/opencv.hpp>
#include <qmdisubwindow.h>
#include <QThread>
#include <QSettings>
#include <QPointer>
#define MAX_NUM_CAM 3
#include "newproject.h"
#include "captureplant.h"
#include "worker.h"
#include "displayfront.h"
#include "displayside.h"
#include "displaytop.h"
#include "displayshape.h"
#include "editmask.h"
#include "listcaptureplant.h"
#include "displaymask.h"
#include "displayhistogram.h"
#include "welcomescreen.h"
#include "editroi.h"

class capturePlant;
class displayFront;
class displaySide;
class displayTop;
class editMask;
class displayShape;
class displayMask;
class displayHistogram;
class listCapturePlant;
class QThread;
class editROI;

namespace Ui {
  class MainWindow;
}



class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  void updateRecentFilesMenu();
  QToolButton* mToolButtonOpenRecent;
  QMenu* mMenuRecentFiles;


  ~MainWindow();

signals:
    void SendRawFrameTop(cv::Mat frame);
    void SendRawFrameFront(cv::Mat frame);
    void SendRawFrameSide(cv::Mat frame);

public slots:
  void displayFrame(cv::Mat frame, int index);

private slots:
  void on_actionNew_Project_triggered();

  void on_actionOpen_Camera_triggered();

  void on_actionDisplay_Front_Camera_triggered();

  void on_actionDisplay_Side_Camera_triggered();

  void on_actionDisplay_Top_Camera_triggered();

  void on_actionModify_Region_of_Intrest_triggered();

  void on_actionModify_Mask_Properties_triggered();

  void on_actionOpen_Project_triggered();

  void on_actionActivate_Cameras_triggered();

  void on_actionDisplay_Shapes_triggered();

  void on_actionDisplay_Masks_triggered();

  void on_actionDisplay_Histograms_triggered();

  void openRecentFile();

private:
  Ui::MainWindow *ui;

  enum { maxRecentFiles = 8 };
  QAction* mActionRecentFiles[maxRecentFiles];

  QPointer<capturePlant> mCapturePlant;
  QPointer<displayTop> mDisplayTop;
  QPointer<displayFront> mDisplayFront;
  QPointer<displaySide> mDisplaySide;
  QPointer<editMask> mEditMask;
  QPointer<displayShape> mDisplayShape;
  QPointer<displayMask> mDisplayMask;
  QPointer<displayHistogram> mDisplayHistogram;
  QPointer<listCapturePlant> mListCapturePlant;
  QPointer<editROI> mEditROI;


  int numCams;
  QThread* threads[MAX_NUM_CAM];
  Worker* workers[MAX_NUM_CAM];
  cv::Mat RawFrameTop;
  cv::Mat RawFrameFront;
  cv::Mat RawFrameSide;

  QString Qr;
  QString List;
  QString Manual;

  QString ProjectDir;
  QString ProjectName;
  QString FileProjectDir;

  QSettings* mInternal;
  QSettings* mProject;


  void loadSubWindow(QWidget *widget);

  void setRecentFile(QString);
  void removeRecentFile(QString);
  void loadFile(QString);

  QAction* mActionSeparatorRecentFiles;
};

#endif // MAINWINDOW_H
