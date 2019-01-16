#include "computervision.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <utility>
#include <cmath>
#include <Eigen/Dense>
#include <QSettings>
#include <QFile>

using namespace cv;
using namespace std;
using namespace Eigen;


float extractRGB_chips(Mat &img,Mat &mask){
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

MatrixXd getRGBarray(Mat img, QString CurrentView){
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
    temp = fileDir+CurrentView;

    for(unsigned int i=1;i<23;i++){
        stringstream ss;
        ss << i;
        string str = ss.str();
        string file_name = temp.toUtf8().constData()+str+".jpg";
        Mat mask = imread(file_name,0);
        Mat cc;
        threshold(mask,cc,90,255,THRESH_BINARY);

        float b_avg = extractRGB_chips(b, cc);
        float g_avg = extractRGB_chips(g, cc);
        float r_avg = extractRGB_chips(r, cc);
        sourceColors(i-1,0) = b_avg;
        sourceColors(i-1,1) = g_avg;
        sourceColors(i-1,2) = r_avg;
    }
    return(sourceColors);
}

void get_standardizations(Mat img, float &det, MatrixXd &rh,MatrixXd &gh,MatrixXd &bh, QString CurrentView){
    //-- Extending source RGB chips to squared and cubic terms
    MatrixXd source1, source2, source3;
    source1 = getRGBarray(img,CurrentView);
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
    QString tempPath = fileDir+CurrentView+"target.csv";

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

Mat color_homography(Mat img, MatrixXd r_coef,MatrixXd g_coef,MatrixXd b_coef){
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


Mat fill_holes(Mat src){
    Mat dst = Mat::zeros(src.size(),src.type());
    vector<vector<Point>>contours;
    vector<Vec4i> hierarchy;
    findContours(src,contours,hierarchy, RETR_TREE,CHAIN_APPROX_SIMPLE,Point(0,0));
    for(unsigned int i = 0; i < contours.size(); i++){
        drawContours(dst,contours,i,255,CV_FILLED);
    }
    return dst;
}
vector<Point> keep_roi(Mat img,const Point& tl, const Point& br, Mat &mask){
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
vector<double> ComputerVision::get_shapes(const vector<Point>& cc,const Mat& mask){
    //-- Get measurements
    Moments mom = moments(mask,true);
    double area = mom.m00;
    convexHull( Mat(cc), hull, false );
    double hull_verticies = hull.size();
    double hull_area = contourArea(Mat(hull));
    double solidity = area/hull_area;
    double perimeter = arcLength(Mat(cc),false);
    cmx = mom.m10 / mom.m00;
    cmy = mom.m01 / mom.m00;
    Rect boundRect = boundingRect( cc );
    width = boundRect.width;
    height = boundRect.height;
    x = boundRect.x;
    y = boundRect.y;
    double circ = 4*M_PI*area/(perimeter*perimeter);
    double angle = -1;
    double ex = -1;
    double ey = -1;
    double emajor = -1;
    double eminor = -1;
    double eccen = -1;
    double round = -1;
    double ar = -1;
    if(cc.size() >= 6){
        Mat pointsf;
        Mat(cc).convertTo(pointsf, CV_32F);
        RotatedRect ellipse = fitEllipse(pointsf);
        angle = ellipse.angle;
        ex = ellipse.center.x;
        ey = ellipse.center.y;
        if(ellipse.size.height > ellipse.size.width){
            emajor = ellipse.size.height;
            eminor = ellipse.size.width;
        }else{
            eminor = ellipse.size.height;
            emajor = ellipse.size.width;
        }
        eccen = sqrt((1- eminor / emajor)*2);
        round = eminor/emajor;
        ar = emajor/eminor;
    }
    double shapes[20] = {area,hull_area,solidity,perimeter,width,height,cmx,cmy,hull_verticies,ex,ey,emajor,eminor,angle,eccen,circ,round,ar,x,y};
    vector<double> shapes_v(shapes,shapes+20);
    return shapes_v;
}

Mat ComputerVision::drawShapes(Mat org, const vector<Point>& cc){
    vector<vector<Point>> tmp;
    vector<vector<Point>> tmp1;
    tmp.push_back(hull);
    tmp1.push_back(cc);
    drawContours(org, tmp1,-1,Scalar(0,0,255),1);
    drawContours(org, tmp,-1,Scalar(255,0,0),1);
    cv::line(org, Point(x,y), Point(x+width,y), Scalar(0,0,255),2);
    cv::line(org,Point(cmx,y),Point(cmx,y+height),Scalar(0,0,255),2);
    cv::circle(org, Point(cmx,cmy), 10, Scalar(0,0,255),2);
    return org;
}

Mat ComputerVision::get_color_Hue(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2HSV);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[0],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_Saturation(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2HSV);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[1],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_Value(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2HSV);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[2],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_R(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2RGB);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[0],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_G(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2RGB);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[1],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_B(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2RGB);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[2],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_L(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2Lab);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[0],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_GM(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2Lab);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[1],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}
Mat ComputerVision::get_color_BY(const Mat& img,const Mat& mask){
    Mat composite;
    cvtColor(img,composite,COLOR_BGR2Lab);
    vector<Mat> channels1;
    split(composite, channels1);
    Mat hist;
    int dims = 1;
    int histSize = 256;
    float hranges[] = { 0, 255 };
    const float *ranges = {hranges};

    //-- Compute the histogram
    calcHist(&channels1[2],1,nullptr,mask,hist, dims, &histSize, &ranges    ,true ,false);

    return hist;
}

Mat ComputerVision::get_RGB_HIST(const Mat& img, const Mat& mask){

  const cv::Scalar background_color(0,0,0);

    const int height = 225;
    const int width = 400;
    const int histogram_size = 256;

    const int margin = 3;
    const int min_y = margin;
    const int max_y = height - margin;
    const int thickness = 1;
    const int line_type = LINE_AA;
    const float bin_width = static_cast<float>(width)/static_cast<float>(histogram_size);
    Mat dst(height,width,CV_8UC3,background_color);
    cv::Scalar colors[] =
    {
        {255,0,0},//red
        {0,255,0},//green
        {0,0,255}, //blue
        {255,0,216}, //Magenta
        {242,255,0}, //Yellow
        {255,0,191}, //Hue
        {0,255,255}, //Saturation
        {255,123,0} //Value
    };

    for (size_t idx=0; idx < 7; idx++)
    {
        //const cv::Scalar colour = colors[idx % 3];
        const cv::Scalar colour = colors[idx];
        cv::Mat histogram;
        if(idx==0){
            histogram = get_color_R(img,mask);
          }
        if(idx==1){
            histogram = get_color_G(img,mask);
          }
        if(idx==2){
            histogram = get_color_B(img,mask);
          }
        if(idx==3){
            histogram = get_color_GM(img,mask);
          }
        if(idx==4){
            histogram = get_color_BY(img,mask);
          }
        if(idx==5){
            histogram = get_color_Hue(img,mask);
          }
        if(idx==6){
            histogram = get_color_Saturation(img,mask);
          }
        if(idx==7){
            histogram = get_color_Value(img,mask);
          }



        cv::normalize(histogram, histogram, 0, dst.rows, cv::NORM_MINMAX);

        for (int i = 1; i < histogram_size; i++)
        {
            const int x1 = std::round(bin_width * (i - 1));
            const int x2 = std::round(bin_width * (i - 0));

            const int y1 = std::clamp(height - static_cast<int>(std::round(histogram.at<float>(i - 1))), min_y, max_y);
            const int y2 = std::clamp(height - static_cast<int>(std::round(histogram.at<float>(i - 0))), min_y, max_y);

            cv::line(dst, cv::Point(x1, y1), cv::Point(x2, y2), colour, thickness, line_type);
        }
    }

    return dst;




}

Mat ComputerVision::remove_background(const Mat& img, const Mat& blank, int blurKM,int tLowM,int tHighM,int b1LM,int b1HM,int b2LM,int b2HM,int x1, int y1, int x2, int y2, QString CurrentView, bool ColorStandardization ){

  Mat adjImg;

  if(ColorStandardization){



      float det = 0;

      MatrixXd rh, gh, bh;

      get_standardizations(img,det,rh,gh,bh,CurrentView);

      adjImg = color_homography(img,rh,gh,bh);

    }
  if(!ColorStandardization){
      adjImg = img;
    }


  Mat dest;
  absdiff(blank,adjImg,dest);
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
  Mat pot_roi;

  vector<Point> cc_pot = keep_roi(pot_and,Point(x1,y1),Point(x2,y2),pot_roi);

  return pot_roi;

}

vector<Point> ComputerVision::get_cc(Mat img,int x1, int y1, int x2, int y2){
    Mat temp;
    vector<Point> cc_pot = keep_roi(std::move(img),Point(x1,y1),Point(x2,y2),temp);
    return cc_pot;

}
