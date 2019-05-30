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
#include <QDebug>

using namespace cv;
using namespace std;
using namespace Eigen;

float get_fd(Mat mask){
	Mat img_bc;
	resize(mask, img_bc, Size(2048,2048), 0, 0, INTER_LINEAR);

	double width = 2048.0;
	double p = log(width)/log(double(2.0));
	VectorXf N = VectorXf::Zero(int(p)+1);
	double sumImg = sum(img_bc)[0];
	N(int(p)) = sumImg;

	double siz;
	double siz2;
	float running_sum;
    for (int g = int(p)-1; g > 0; g--){
        siz = pow(2.0, double(p-g));
        siz2 = round(siz/2.0);
        running_sum = 0;
        for (int i = 0; i < int(width-siz+1); i = i+int(siz)){
                for (int j = 0; j < int(width-siz+1); j = j+int(siz)){
                        img_bc.at<uchar>(i,j) = (bool(img_bc.at<uchar>(i,j)) || bool(img_bc.at<uchar>(i+siz2,j))
                                || bool(img_bc.at<uchar>(i,j+siz2)) || bool(img_bc.at<uchar>(i+siz2,j+siz2)));
                        running_sum = running_sum+float(img_bc.at<uchar>(i,j));
                }
        }
        N(g) = running_sum;
        }
    N = N.colwise().reverse().eval();

    VectorXf R = VectorXf::Zero(int(p)+1);
    R(0) = 1.0;
    for (int k = 1; k < R.size(); k++){
        R(k) = pow(2.0, double(k));
    }

        float slope [R.size()-1];
        for(int i=1;i < R.size()-1 ;i++){
                slope[i] = (log10(N(i+1))-log10(N(i)))/(log10(R(i+1))-log10(R(i)));
        }

	//-- Getting average slope (fractal dimension)
	float sum = 0.0, average;
	int s_count =0;
	for(int i=1; i < R.size()-1; i++){
		if(-slope[i] < 2 && -slope[i] > 0){
			sum += -slope[i];
			s_count++;
		}
	}
	average = sum / s_count;
	return average;
}

int is_oof(Mat img){
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
    findContours( img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    Mat src = Mat::zeros(img.size(),img.type())+255;

    vector<vector<Point> > contours_roi;
    vector<Vec4i> hierarchy_roi;
    findContours( src, contours_roi, hierarchy_roi, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    int check = 0;
    for(unsigned int i=0; i < contours.size(); i++){
        for(unsigned int j=0; j<contours[i].size(); j++){
                int test = pointPolygonTest(contours_roi[0],Point2f(contours[i][j]),false);
                if(test == 0){
                        check = 1;
                }
        }
    }
        return check;
}

void thinningIteration(cv::Mat& img, int iter)
{
    Mat marker = Mat::zeros(img.size(),CV_8UC1);

    for (int i = 1; i < img.rows-1; i++)
    {
        for (int j = 1; j < img.cols-1; j++)
        {
            uchar p2 = img.at<uchar>(i-1, j);
            uchar p3 = img.at<uchar>(i-1, j+1);
            uchar p4 = img.at<uchar>(i, j+1);
            uchar p5 = img.at<uchar>(i+1, j+1);
            uchar p6 = img.at<uchar>(i+1, j);
            uchar p7 = img.at<uchar>(i+1, j-1);
            uchar p8 = img.at<uchar>(i, j-1);
            uchar p9 = img.at<uchar>(i-1, j-1);

            int C  = ((!p2) & (p3 | p4)) + ((!p4) & (p5 | p6)) +
            ((!p6) & (p7 | p8)) + ((!p8) & (p9 | p2));
            int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
            int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
            int N  = N1 < N2 ? N1 : N2;
            int m  = iter == 0 ? ((p6 | p7 | (!p9)) & p8) : ((p2 | p3 | (!p5)) & p4);

            if ((C == 1) && ((N >= 2) && ((N <= 3)) & (m == 0)))
                marker.at<uchar>(i,j) = 1;
        }
    }

    img &= ~marker;
}

void thinning(const cv::Mat& src, cv::Mat& dst)
{
    dst = src.clone();
    dst /= 255;         // convert to binary image

    cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningIteration(dst, 0);
        thinningIteration(dst, 1);
        cv::absdiff(dst, prev, diff);
        dst.copyTo(prev);
    }
    while (cv::countNonZero(diff) > 0);

    dst *= 255;
}

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
    if(area>0){
        vector<Point>hull;
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
    float fd = get_fd(mask);
    double oof = is_oof(mask);
    double shapes[22] = {area,hull_area,solidity,perimeter,width,height,cmx,cmy,hull_verticies,ex,ey,emajor,eminor,angle,eccen,circ,round,ar,x,y,fd,oof};
    vector<double> shapes_v(shapes,shapes+22);
    return shapes_v;
      }else{
        double shapes[22] = {0,0,-nan("1"),0,0,0,-nan("1"),-nan("1"),0,-1,-1,-1,-1,-1,-1,-nan("1"),-1,-1,0,0,-nan("1"),0};
        vector<double> shapes_v(shapes,shapes+22);
        return shapes_v;
      }
}

Mat ComputerVision::drawShapes(Mat org, const vector<Point>& cc){
    vector<vector<Point>> tmp;
    vector<vector<Point>> tmp1;
    qDebug()<<"CV_2A";
    tmp.push_back(hull);
    qDebug()<<"CV_2A1";
    tmp1.push_back(cc);
    qDebug()<<"CV_2A2";
    drawContours(org, tmp1,-1,Scalar(0,0,255),1);
    qDebug()<<"CV_2A3";
    drawContours(org, tmp,-1,Scalar(255,0,0),1);
    qDebug()<<"CV_2b";
    cv::line(org, Point(x,y), Point(x+width,y), Scalar(0,0,255),2);
    qDebug()<<"CV_2c";

    cv::line(org,Point(cmx,y),Point(cmx,y+height),Scalar(0,0,255),2);
    qDebug()<<"CV_2d";

    cv::circle(org, Point(cmx,cmy), 10, Scalar(0,0,255),2);
    qDebug()<<"CV_2f";

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

Mat ComputerVision::remove_background(const Mat& img, const Mat& blank, int blurKM,int tLowM,int tHighM,int b1LM,int b1HM,int b2LM,int b2HM,int x1,
                                      int y1, int x2, int y2, QString CurrentView, bool ColorStandardization,int BluePlantThreshold, int BluePlantBlur, int GreenPlantThreshold, int GreenPlantBlur, int MaskAlphaThresholdDark,
                                      int MaskAlphaThresholdLight,int MaskBetaThreshold,int DifferenceDilateKernelSize, int DifferenceErodeKernelSize,
                                      int PotDilateKernelSize, int PotErodeKernelSize){

  Mat adjImg;

  if(ColorStandardization==true){



      float det = 0;

      MatrixXd rh, gh, bh;

      get_standardizations(img,det,rh,gh,bh,CurrentView);

      adjImg = color_homography(img,rh,gh,bh);

    }
  if(ColorStandardization==false){
      adjImg = img;
    }
  //imwrite("/Users/dnguyen/Desktop/adjImg.png",adjImg);
  //imwrite("/Users/dnguyen/Desktop/blank.png",blank);

  qDebug()<<"Crash CV1";
  Mat dest;
  absdiff(blank,adjImg,dest);
  qDebug()<<"Crash CV1a";
  vector<Mat> channels(3);
    qDebug()<<"Crash CV1b";
  split(dest,channels);
    qDebug()<<"Crash CV1c";
  Mat dest_blur;
  blur(channels[1],dest_blur,Size(2,2));
    qDebug()<<"Crash CV1d";
  Mat dest_thresh;
  threshold(dest_blur,dest_thresh,25,255,THRESH_BINARY);
    qDebug()<<"Crash CV1e";
  Mat dest_dilate;
  dilate(dest_thresh,dest_dilate,Mat(), Point(-1,-1),5,1,1);
    qDebug()<<"Crash CV1f";
  Mat dest_erode;
  erode(dest_dilate,dest_erode,Mat(),Point(-1,-1),4,1,1);
  qDebug()<<"Crash CV2";
  Mat plantDest;
  adjImg.copyTo(plantDest,dest_erode);
  qDebug()<<"Crash CV3";
  Mat lab;
  Mat adjImage1 = plantDest.clone();
  cvtColor(adjImage1, lab, cv::COLOR_BGR2Lab);
  vector<Mat> split_lab;
  split(lab,split_lab);
  Mat mask_b;
  threshold(split_lab[2], mask_b, 135, 255,THRESH_BINARY);
  medianBlur(mask_b, mask_b, 3);
  mask_b=fill_holes(mask_b);
  Mat mask_b_dilate;
  Mat mask_b_erode;
  dilate(mask_b,mask_b_dilate,Mat(), Point(-1,-1),3,1,1);
  erode(mask_b_dilate,mask_b_erode,Mat(),Point(-1,-1),1,1,1);
   qDebug()<<"Crash CV4";
  Mat hsv;
  cvtColor(adjImage1, hsv, cv::COLOR_BGR2HSV);
  vector<Mat> split_hsv;
  split(hsv, split_hsv);
  Mat mask_s;
  threshold(split_hsv[1], mask_s, 65, 255, THRESH_BINARY);
  //medianBlur(mask_s, mask_s, GreenPlantBlur);
  mask_s=fill_holes(mask_s);
  Mat mask_s_dilate;
  Mat mask_s_erode;
  dilate(mask_s,mask_s_dilate,Mat(),Point(-1,-1),3,1,1);
  erode(mask_s_dilate,mask_s_erode,Mat(),Point(-1,-1),1,1,1);
   qDebug()<<"Crash CV5";
  Mat mask_erode;
  bitwise_or(mask_b_erode, mask_s_erode, mask_erode);
  vector<Point> cc_PreLim = keep_roi(mask_erode,Point(x1,y1),Point(x2,y2),mask_erode);
 qDebug()<<"Crash CV6";
  Mat masked;
  adjImg.copyTo(masked,mask_erode);
  Mat masked_a;
  cvtColor(masked, masked_a, cv::COLOR_BGR2Lab);
  vector<Mat> split_lab_a;
  split(masked_a,split_lab_a);
  Mat maskeda_thresh;
  threshold(split_lab_a[1], maskeda_thresh, 115, 255, THRESH_BINARY_INV);
  Mat maskeda_thresh1;
  threshold(split_lab_a[1], maskeda_thresh1, 135, 255, THRESH_BINARY);
  Mat maskedb_thresh;
  threshold(split_lab_a[2], maskedb_thresh, 128, 255, THRESH_BINARY);
  Mat ab1;
  Mat ab;
  bitwise_or(maskeda_thresh, maskedb_thresh, ab1);
  bitwise_or(maskeda_thresh1, ab1, ab);
  Mat fill1;
  fill1 = fill_holes(ab);
   qDebug()<<"Crash CV7";
  Mat mask;

  vector<Point> cc = keep_roi(fill1,Point(x1,y1),Point(x2,y2),mask);
  medianBlur(mask,mask,3);
   qDebug()<<"Crash CV8";
  return mask;

}

vector<Point> ComputerVision::get_cc(Mat img,int x1, int y1, int x2, int y2){
    Mat temp;
    vector<Point> cc_pot = keep_roi(std::move(img),Point(x1,y1),Point(x2,y2),temp);
    return cc_pot;

}

vector<int> ComputerVision::getTips(const Mat &img){

  medianBlur(img, img, 3);


      Mat thinning_dilate;
      dilate(img, thinning_dilate, Mat(), Point(-1, -1), 3, 1, 1);
      Mat thinning_erode;
      erode(thinning_dilate,thinning_erode, Mat(), Point(-1, -1), 1, 1, 1);

      thinning(thinning_erode,thinning_erode );

      // Declare variable to count neighbourhood pixels
      int count;

      // To store a pixel intensity
      uchar pix;

      // To store the ending co-ordinates
      std::vector<int> coords;

      // For each pixel in our image...
      for (int i = 1; i < thinning_erode.rows-1; i++) {
          for (int j = 1; j < thinning_erode.cols-1; j++) {

              // See what the pixel is at this location
              pix = thinning_erode.at<uchar>(i,j);

              // If not a skeleton point, skip
              if (pix == 0)
                  continue;

              // Reset counter
              count = 0;

              // For each pixel in the neighbourhood
              // centered at this skeleton location...
              for (int y = -1; y <= 1; y++) {
                  for (int x = -1; x <= 1; x++) {

                      // Get the pixel in the neighbourhood
                      pix = thinning_erode.at<uchar>(i+y,j+x);

                      // Count if non-zero
                      if (pix != 0)
                          count++;
                  }
              }

              // If count is exactly 2, add co-ordinates to vector
              if (count == 2) {
                  coords.push_back(i);
                  coords.push_back(j);
              }

          }
      }

      std::vector<int> coordsX;
      std::vector<int> coordsY;
      for (int i = 0; i < coords.size() / 2; i++){
          int x = coords.at(2*i);
          int y = coords.at(2*i+1);
          coordsX.push_back(x);
          coordsY.push_back(y);
      }

      int size = coordsX.size();
      for (int i = 0; i < size-1; ++i)
      {
          if (size != (int)coordsX.size())
          {
              --i;
              size = coordsX.size();
              printf("Go back %d\n",size);
          }
          assert(i > -1 && i < (int)coordsX.size());
          if(coordsX[i] + 5 >= coordsX[i+1])
          {
              printf("Removing %d, %d\n",coordsX[i],i);
              coordsX.erase(coordsX.begin() + i);
              coordsY.erase(coordsY.begin()+i);
          }


      }

      std::vector<int> coordsXY;


      for (int i = 0; i< size; ++i){
          cout << coordsX.at(i) << ","<<coordsY.at(i)<<"\n";
          coordsXY.push_back(coordsX.at(i));
          coordsXY.push_back(coordsY.at(i));
      }

      return coordsXY;

}

vector<int> ComputerVision::getNodes(const Mat &img){

  medianBlur(img, img, 3);


      Mat thinning_dilate;
      dilate(img, thinning_dilate, Mat(), Point(-1, -1), 3, 1, 1);
      Mat thinning_erode;
      erode(thinning_dilate,thinning_erode, Mat(), Point(-1, -1), 1, 1, 1);

      thinning(thinning_erode,thinning_erode );



      // Declare variable to count neighbourhood pixels
      int count;

      // To store a pixel intensity
      uchar pix;

      // To store the ending co-ordinates
      std::vector<int> coords;

      // For each pixel in our image...
      for (int i = 1; i < thinning_erode.rows-1; i++) {
          for (int j = 1; j < thinning_erode.cols-1; j++) {

              // See what the pixel is at this location
              pix = thinning_erode.at<uchar>(i,j);

              // If not a skeleton point, skip
              if (pix == 0)
                  continue;

              // Reset counter
              count = 0;

              // For each pixel in the neighbourhood
              // centered at this skeleton location...
              for (int y = -1; y <= 1; y++) {
                  for (int x = -1; x <= 1; x++) {

                      // Get the pixel in the neighbourhood
                      pix = thinning_erode.at<uchar>(i+y,j+x);

                      // Count if non-zero
                      if (pix != 0)
                          count++;
                  }
              }

              // If count is exactly 4, add co-ordinates to vector

              if (count == 4) {
                  coords.push_back(i);
                  coords.push_back(j);
              }

          }
      }

      std::vector<int> coordsX;
      std::vector<int> coordsY;
      for (int i = 0; i < coords.size() / 2; i++){
          int x = coords.at(2*i);
          int y = coords.at(2*i+1);
          coordsX.push_back(x);
          coordsY.push_back(y);
      }

      int size = coordsX.size();
      for (int i = 0; i < size-1; ++i)
      {
          if (size != (int)coordsX.size())
          {
              --i;
              size = coordsX.size();
              printf("Go back %d\n",size);
          }
          assert(i > -1 && i < (int)coordsX.size());
          if(coordsX[i] + 5 >= coordsX[i+1])
          {
              printf("Removing %d, %d\n",coordsX[i],i);
              coordsX.erase(coordsX.begin() + i);
              coordsY.erase(coordsY.begin()+i);
          }


      }

      std::vector<int> coordsXY;


      for (int i = 0; i< size; ++i){
          cout << coordsX.at(i) << ","<<coordsY.at(i)<<"\n";
          coordsXY.push_back(coordsX.at(i));
          coordsXY.push_back(coordsY.at(i));
      }

      return coordsXY;

}
