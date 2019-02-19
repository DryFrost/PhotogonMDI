#include "manualimportmask.h"
#include "ui_manualimportmask.h"
#include <QSettings>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <utility>
#include <cmath>
#include <Eigen/Dense>
#include <QFileDialog>
using namespace Eigen;
using namespace cv;
using namespace std;



ManualImportMask::ManualImportMask(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ManualImportMask)
{
  ui->setupUi(this);
  show();
  timer = new QTimer(this);
  QSettings internal("internal.ini",QSettings::IniFormat);
  ManualColorStandard = internal.value("ManualColorStandard").value<bool>();
  ManualBackgroundPath = internal.value("FtLowM").value<QString>();
  BluePlantThreshold = internal.value("BluePlantThresholdFront").value<int>();
  BluePlantBlur = internal.value("BluePlantBlurFront").value<int>();
  GreenPlantThreshold = internal.value("GreenPlantThresholdFront").value<int>();
  GreenPlantBlur = internal.value("GreenPlantBlurFront").value<int>();
  MaskAlphaThresholdDark = internal.value("MaskAlphaThresholdDarkFront").value<int>();
  MaskAlphaThresholdLight = internal.value("MaskAlphaThresholdLightFront").value<int>();
  MaskBetaThreshold = internal.value("MaskBetaThresholdFront").value<int>();
  PotDilateKernelSize = internal.value("PotDilateKernelSizeFront").value<int>();
  PotErodeKernelSize = internal.value("PotErodeKernelSizeFront").value<int>();
  x1 = internal.value("x1Front").value<int>();
  x2 = internal.value("x2Front").value<int>();
  y1 = internal.value("y1Front").value<int>();
  y2 = internal.value("y2Front").value<int>();
  b1LM = internal.value("Fb1LM").value<int>();
  b1HM = internal.value("Fb1HM").value<int>();
  b2LM = internal.value("Fb2LM").value<int>();
  b2HM = internal.value("Fb2HM").value<int>();

  ui->bYFPL->setSliderPosition(b1LM);
  ui->bYSPL->setSliderPosition(b2LM);
  ui->bYFPH->setSliderPosition(b1HM);
  ui->bYSPH->setSliderPosition(b2HM);

  ui->BluePlantThreshold->setSliderPosition(BluePlantThreshold);
  ui->BluePlantBlur->setSliderPosition(BluePlantBlur);
  ui->GreenPlantThreshold->setSliderPosition(GreenPlantThreshold);
  ui->GreenPlantBlur->setSliderPosition(GreenPlantBlur);
  ui->MaskAlphaThresholdDark->setSliderPosition(MaskAlphaThresholdDark);
  ui->MaskAlphaThresholdLight->setSliderPosition(MaskAlphaThresholdLight);
  ui->MaskBetaThreshold->setSliderPosition(MaskBetaThreshold);
  ui->PotDilateKernelSize->setSliderPosition(PotDilateKernelSize);
  ui->PotErodeKernelSize->setSliderPosition(PotErodeKernelSize);
}

ManualImportMask::~ManualImportMask()
{
  delete ui;
}

void ManualImportMask::on_pushButton_clicked()
{

  PlantPic = QFileDialog::getOpenFileName(this, tr("Files"), QDir::currentPath(), tr("*.jpg *.png"));
  currentFrame = imread(PlantPic.toUtf8().constData());
  blank = imread(ManualBackgroundPath.toUtf8().constData());
  connect(timer,SIGNAL(timeout()),this,SLOT(update_window()));
  timer->start(20);

}

Mat fill_holes3(Mat src){
    Mat dst = Mat::zeros(src.size(),src.type());
    vector<vector<Point>>contours;
    vector<Vec4i> hierarchy;
    findContours(src,contours,hierarchy, RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));
    for(unsigned int i = 0; i < contours.size(); i++){
        drawContours(dst,contours,i,255,CV_FILLED);
    }
    return dst;
}

vector<Point> keep_roi3(Mat img,const Point& tl, const Point& br, Mat &mask){
    //-- Get contours of mask
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //-- Get contours of rectangular roi
    Mat src = Mat::zeros(img.size(),img.type());
    rectangle(src,tl,br,255,CV_FILLED);

    vector<vector<Point> > contours_roi;
    vector<Vec4i> hierarchy_roi;
    findContours( src, contours_roi, hierarchy_roi, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //-- Keep only those contours that have a point inside roi
    vector<Point> cc;
    Mat kept = Mat::zeros(img.size(),img.type());
    for(unsigned int i=0; i < contours.size(); i++){
        for(unsigned int j=0; j<contours[i].size(); j++){
            int test = pointPolygonTest(contours_roi[0],Point2f(contours[i][j]),false);
            if(test==1 || test == 0){
                for(const auto & k : contours[i]){
                    cc.push_back(k);
                }
                drawContours(kept, contours, i, 255, CV_FILLED);
                break;
            }
        }
    }
    Mat kept_mask;
    bitwise_and(img,kept,kept_mask);

    mask = kept_mask;
    return cc;
}


float extractRGB_chips3(Mat &img,Mat &mask){
    //-- Averages the histogram for a given channel
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 256 };
    const float *ranges = {hranges};

    calcHist(&img,1,0,mask,hist, dims, &histSize, &ranges ,true ,false);

    int sum=0;
    for(int i = 0;i<256;i++){
        sum += hist.at<float>(i,0);
    }
    Mat weights = hist/sum;
    float hist_avg=0.0;
    for(int i = 0;i<256;i++){
        hist_avg += i*weights.at<float>(i,0);
    }


    return hist_avg;
}

MatrixXd getRGBarray3(Mat img){
    //-- Loops over chips and gets RGB values of each one
    MatrixXd sourceColors(22,3);
    vector<Mat> bgr;
    split(img, bgr);
    Mat b = bgr[0];
    Mat g = bgr[1];
    Mat r = bgr[2];

    QSettings internal("internal.ini",QSettings::IniFormat);
    QString ProjectDirA = internal.value("ProjectDir").value<QString>();
    QSettings setup(ProjectDirA,QSettings::IniFormat);
    QString ProjectDir = setup.value("ProjectDir").value<QString>();

    QString Path;


    QString fileDir = ProjectDir+"/ColorMasks/";

    QString temp;
    temp = fileDir;

    for(unsigned int i=1;i<23;i++){
        stringstream ss;
        ss << i;
        string str = ss.str();
        string file_name = temp.toUtf8().constData()+str+".jpg";
        Mat mask = imread(file_name,0);
        Mat cc;
        threshold(mask,cc,90,255,THRESH_BINARY);

        float b_avg = extractRGB_chips3(b, cc);
        float g_avg = extractRGB_chips3(g, cc);
        float r_avg = extractRGB_chips3(r, cc);
        sourceColors(i-1,0) = b_avg;
        sourceColors(i-1,1) = g_avg;
        sourceColors(i-1,2) = r_avg;
    }
    return(sourceColors);
}

void get_standardizations3(Mat img, float &det, MatrixXd &rh,MatrixXd &gh,MatrixXd &bh){
    //-- Extending source RGB chips to squared and cubic terms
    MatrixXd source1, source2, source3;
    source1 = getRGBarray3(img);
    source2 = (source1.array() * source1.array()).matrix();
    source3 = (source2.array() * source1.array()).matrix();
    MatrixXd source(source1.rows(),source1.cols()+source2.cols()+source3.cols());
    source << source1, source2, source3;
    //-- Computing Moore-Penrose Inverse
    MatrixXd M = (source.transpose()*source).inverse()*source.transpose();

    //-- Reading target homography
    MatrixXd target(22,3);
    fstream file;

    QSettings internal("internal.ini",QSettings::IniFormat);
    QString ProjectDirA = internal.value("ProjectDir").value<QString>();
    QSettings setup(ProjectDirA,QSettings::IniFormat);
    QString ProjectDir = setup.value("ProjectDir").value<QString>();
    QString fileDir = ProjectDir+"/ColorMasks/";
    QString tempPath = fileDir+"target.csv";

    file.open(tempPath.toUtf8().constData());
    string value;
    int rowCounter = 0;
    while ( getline ( file, value) )
    {
        vector<float> result;
        stringstream substr(value);
        string item;
        while (getline(substr, item, ',')) {
            const char *cstr = item.c_str();
            char* pend;
            float num = strtof(cstr,&pend);
            result.push_back(num);
        }
        target(rowCounter,0) = result[0];
        target(rowCounter,1) = result[1];
        target(rowCounter,2) = result[2];
        rowCounter++;
    }

    //-- Computing linear target RGB standardizations
    rh = M*target.col(0);
    gh = M*target.col(1);
    bh = M*target.col(2);


    //-- Extending target RGB chips to squared and cubic terms
    MatrixXd target1, target2, target3;
    target2 = (target.array() * target.array()).matrix();
    target3 = (target2.array() * target.array()).matrix();

    //-- Computing square and cubic target RGB standardizations
    MatrixXd r2h,g2h,b2h,r3h,g3h,b3h;
    r2h = M*target2.col(0);
    g2h = M*target2.col(1);
    b2h = M*target2.col(2);
    r3h = M*target3.col(0);
    g3h = M*target3.col(1);
    b3h = M*target3.col(2);

    //-- Computing D
    MatrixXd H(9,9);
    H << bh.col(0),gh.col(0),rh.col(0),b2h.col(0),g2h.col(0),r2h.col(0),b3h.col(0),g3h.col(0),r3h.col(0);
    det = H.transpose().determinant();
}

Mat color_homography3(Mat img, MatrixXd r_coef,MatrixXd g_coef,MatrixXd b_coef){
    Mat b, g, r, b2, g2, r2, b3, g3, r3;
    vector<Mat> bgr(3);
    split(img,bgr);

    //-- Computing linear, squared, and cubed images
    b = bgr[0];
    g = bgr[1];
    r = bgr[2];
    b2 = b.mul(b);
    g2 = g.mul(g);
    r2 = r.mul(r);
    b3 = b2.mul(b);
    g3 = g2.mul(g);
    r3 = r2.mul(r);

    //-- Computing homography
    b = 0+r*b_coef(0,0)+g*b_coef(1,0)+b*b_coef(2,0)+r2*b_coef(3,0)+g2*b_coef(4,0)+b2*b_coef(5,0)+r3*b_coef(6,0)+g3*b_coef(7,0)+b3*b_coef(8,0);
    g = 0+r*g_coef(0,0)+g*g_coef(1,0)+b*g_coef(2,0)+r2*g_coef(3,0)+g2*g_coef(4,0)+b2*g_coef(5,0)+r3*g_coef(6,0)+g3*g_coef(7,0)+b3*g_coef(8,0);
    r = 0+r*r_coef(0,0)+g*r_coef(1,0)+b*r_coef(2,0)+r2*r_coef(3,0)+g2*r_coef(4,0)+b2*r_coef(5,0)+r3*r_coef(6,0)+g3*r_coef(7,0)+b3*r_coef(8,0);

    //-- Combining channels and returning
    bgr[0] = b;
    bgr[1] = g;
    bgr[2] = r;
    Mat adjImage;
    merge(bgr,adjImage);
    return adjImage;
}

void ManualImportMask::update_window(){


  b1LM = ui->bYFPL->value();
  b1HM = ui->bYFPH->value();
  b2LM = ui->bYSPL->value();
  b2HM = ui->bYSPH->value();

  BluePlantThreshold = ui->BluePlantThreshold->value();
  BluePlantBlur = ui->BluePlantBlur->value();
  if (BluePlantBlur==2){
      BluePlantBlur=1;
    }
  if (BluePlantBlur==4){
      BluePlantBlur=3;
    }
  if (BluePlantBlur==6){
      BluePlantBlur=5;
    }
  if (BluePlantBlur==8){
      BluePlantBlur=7;
    }
  GreenPlantThreshold = ui->GreenPlantThreshold->value();
  GreenPlantBlur = ui->GreenPlantBlur->value();
  if (GreenPlantBlur==2){
      GreenPlantBlur=1;
    }
  if (GreenPlantBlur==4){
      GreenPlantBlur=3;
    }
  if (GreenPlantBlur==6){
      GreenPlantBlur=5;
    }
  if (GreenPlantBlur==8){
      GreenPlantBlur=7;
    }
  MaskAlphaThresholdDark = ui->MaskAlphaThresholdDark->value();
  MaskAlphaThresholdLight = ui->MaskAlphaThresholdLight->value();
  MaskBetaThreshold = ui->MaskBetaThreshold->value();
  PotDilateKernelSize = ui->PotDilateKernelSize->value();
  PotErodeKernelSize = ui->PotErodeKernelSize->value();

  Mat adjImg;

  if(ManualColorStandard){

      float det = 0;

      MatrixXd rh, gh, bh;

      get_standardizations3(currentFrame,det,rh,gh,bh);

      adjImg = color_homography3(currentFrame,rh,gh,bh);

    }
  if(!ManualColorStandard){
      adjImg = currentFrame;
    }


  Mat lab;
  Mat adjImage1 = adjImg.clone();
  cvtColor(adjImage1, lab, cv::COLOR_BGR2Lab);
  vector<Mat> split_lab;
  split(lab,split_lab);
  Mat mask_b;
  threshold(split_lab[2], mask_b, BluePlantThreshold, 255,THRESH_BINARY);
  medianBlur(mask_b, mask_b, BluePlantBlur);
  mask_b=fill_holes3(mask_b);

  Mat hsv;
  cvtColor(adjImage1, hsv, cv::COLOR_BGR2HSV);
  vector<Mat> split_hsv;
  split(hsv, split_hsv);
  Mat mask_s;
  threshold(split_hsv[1], mask_s, GreenPlantThreshold, 255, THRESH_BINARY);
  medianBlur(mask_s, mask_s, GreenPlantBlur);
  mask_s=fill_holes3(mask_s);

  Mat mask_erode;
  bitwise_or(mask_b, mask_s, mask_erode);

  Mat masked;
  adjImg.copyTo(masked,mask_erode);
  Mat masked_a;
  cvtColor(masked, masked_a, cv::COLOR_BGR2Lab);
  vector<Mat> split_lab_a;
  split(masked_a,split_lab_a);
  Mat maskeda_thresh;
  threshold(split_lab_a[1], maskeda_thresh, MaskAlphaThresholdDark, 255, THRESH_BINARY_INV);
  Mat maskeda_thresh1;
  threshold(split_lab_a[1], maskeda_thresh1, MaskAlphaThresholdLight, 255, THRESH_BINARY);
  Mat maskedb_thresh;
  threshold(split_lab_a[2], maskedb_thresh, MaskBetaThreshold, 255, THRESH_BINARY);
  Mat ab1;
  Mat ab;
  bitwise_or(maskeda_thresh, maskedb_thresh, ab1);
  bitwise_or(maskeda_thresh1, ab1, ab);

  Mat dest_lab;
  cvtColor(adjImg,dest_lab,CV_BGR2Lab);
  vector<Mat> channels_lab;
  split(dest_lab,channels_lab);
  Mat pot_thresh1;
  inRange(channels_lab[2],b1LM,b1HM,pot_thresh1);
  Mat pot_thresh2;
  inRange(channels_lab[2],b2LM,b2HM,pot_thresh2);
  Mat pot_or;
  bitwise_or(pot_thresh1,pot_thresh2,pot_or);
  Mat pot_dilate;
  dilate(pot_or, pot_dilate, Mat(), Point(-1, -1), PotDilateKernelSize, 1, 1);
  Mat pot_erode;
  erode(pot_dilate,pot_erode, Mat(), Point(-1, -1), PotErodeKernelSize, 1, 1);
  Mat pot_and;
  bitwise_and(pot_erode,ab,pot_and);
  Mat pot_roi;

  vector<Point> cc_pot = keep_roi3(pot_and,Point(x1,y1),Point(x2,y2),pot_roi);

  Mat inputImage_lab;
  cvtColor(adjImg, inputImage_lab, CV_BGR2Lab);
  vector<Mat> img_channels_lab;
  split(inputImage_lab, img_channels_lab);
  Mat b_thresh;
  inRange(img_channels_lab[2],80,115,b_thresh);
  Mat b_er;
  erode(b_thresh,b_er, Mat(), Point(-1, -1), 1, 1, 1);
  Mat b_roi;
  vector<Point> cc1 = keep_roi3(b_er,Point(x1,y1),Point(x2,y2),b_roi);
  Mat b_dil;
  dilate(b_roi,b_dil,Mat(),Point(-1, -1), 6, 1, 1);
  Mat b_xor = pot_roi - b_dil;


  Mat mask;
  vector<Point> cc = keep_roi3(b_xor,Point(x1,y2),Point(x1,y2),mask);

  Size size(300,200);


  cv::resize(pot_and,pot_and,size);
  QImage qt_imageM = QImage((const unsigned char*)(pot_and.data),pot_and.cols,pot_and.rows,QImage::Format_Indexed8);
  ui->maskM->setPixmap(QPixmap::fromImage(qt_imageM));

}

void ManualImportMask::on_pushButton_4_clicked()
{
  QSettings internal("internal.ini",QSettings::IniFormat);
  internal.setValue("Fb1LM",QVariant::fromValue(b1LM));
  internal.setValue("Fb1HM",QVariant::fromValue(b1HM));
  internal.setValue("Fb2LM",QVariant::fromValue(b2LM));
  internal.setValue("Fb2HM",QVariant::fromValue(b2HM));
  internal.setValue("BluePlantThresholdFront",QVariant::fromValue(BluePlantThreshold));
  internal.setValue("BluePlantBlurFront",QVariant::fromValue(BluePlantBlur));
  internal.setValue("GreenPlantThresholdFront",QVariant::fromValue(GreenPlantThreshold));
  internal.setValue("GreenPlantBlurFront",QVariant::fromValue(GreenPlantBlur));
  internal.setValue("MaskAlphaThresholdDarkFront",QVariant::fromValue(MaskAlphaThresholdDark));
  internal.setValue("MaskAlphaThresholdLightFront",QVariant::fromValue(MaskAlphaThresholdLight));
  internal.setValue("MaskBetaThresholdFront",QVariant::fromValue(MaskBetaThreshold));
  internal.setValue("PotDilateKernelSizeFront",QVariant::fromValue(PotDilateKernelSize));
  internal.setValue("PotErodeKernelSizeFront",QVariant::fromValue(PotErodeKernelSize));
  close();
}

void ManualImportMask::on_pushButton_5_clicked()
{
    close();
}
