#ifndef MANUALCOMPUTERVISION_H
#define MANUALCOMPUTERVISION_H

#include <opencv2/opencv.hpp>
#include <QFile>
using namespace cv;
using namespace std;
class ComputerVisionNoSub{
public:
    Mat remove_backgroundNoSub(const Mat& img,int b1LM,
                          int b1HM,int b2LM,int b2HM,int x1, int y1, int x2, int y2,bool ColorStandardization,
                          int BluePlantThreshold, int BluePlantBlur, int GreenPlantThreshold, int GreenPlantBlur, int MaskAlphaThresholdDark,
                          int MaskAlphaThresholdLight,int MaskBetaThreshold,int PotDilateKernelSize, int PotErodeKernelSize);
    Mat get_RGB_HISTNoSub(const Mat& img, const Mat& mask);
    Mat get_color_RNoSub(const Mat& img, const Mat& mask);
    Mat get_color_GNoSub(const Mat& img, const Mat& mask);
    Mat get_color_BNoSub(const Mat& img, const Mat& mask);
    Mat get_color_LNoSub(const Mat& img, const Mat& mask);
    Mat get_color_GMNoSub(const Mat& img, const Mat& mask);
    Mat get_color_BYNoSub(const Mat& img, const Mat& mask);
    Mat get_color_HueNoSub(const Mat& img, const Mat& mask);
    Mat get_color_SaturationNoSub(const Mat& img, const Mat& mask);
    Mat get_color_ValueNoSub(const Mat& img, const Mat& mask);
    vector<int> getNodesNoSub(const Mat& img);
    vector<int> getTipsNoSub(const Mat& img);

    vector<Point> get_ccNoSub(Mat img, int x1, int y1, int x2, int y2);
    vector<double> get_shapesNoSub(const vector<Point>& cc,const Mat& mask);
    Mat drawShapesNoSub(Mat img, const vector<Point>& cc);
private:
    vector<Point>hull;
    double x;
    double y;
    double width;
    double height;
    double cmx;
    double cmy;

};

#endif // MANUALCOMPUTERVISION_H
