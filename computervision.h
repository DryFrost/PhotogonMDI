#ifndef COMPUTERVISION_H
#define COMPUTERVISION_H


#include <opencv2/opencv.hpp>
#include <QFile>
using namespace cv;
using namespace std;
class ComputerVision
{
public:
    Mat remove_background(const Mat& img, const Mat& blank, int blurKM,int tLowM,int tHighM,int b1LM,
                          int b1HM,int b2LM,int b2HM,int x1, int y1, int x2, int y2,QString CurrentView,bool ColorStandardization,
                          int BluePlantThreshold, int BluePlantBlur, int GreenPlantThreshold, int GreenPlantBlur, int MaskAlphaThresholdDark,
                          int MaskAlphaThresholdLight,int MaskBetaThreshold,int DifferenceDilateKernelSize, int DifferenceErodeKernelSize,int PotDilateKernelSize, int PotErodeKernelSize);
    Mat get_RGB_HIST(const Mat& img, const Mat& mask);
    Mat get_color_R(const Mat& img, const Mat& mask);
    Mat get_color_G(const Mat& img, const Mat& mask);
    Mat get_color_B(const Mat& img, const Mat& mask);
    Mat get_color_L(const Mat& img, const Mat& mask);
    Mat get_color_GM(const Mat& img, const Mat& mask);
    Mat get_color_BY(const Mat& img, const Mat& mask);
    Mat get_color_Hue(const Mat& img, const Mat& mask);
    Mat get_color_Saturation(const Mat& img, const Mat& mask);
    Mat get_color_Value(const Mat& img, const Mat& mask);
    vector<int> getNodes(const Mat& img);
    vector<int> getTips(const Mat& img);

    vector<Point> get_cc(Mat img, int x1, int y1, int x2, int y2);
    vector<double> get_shapes(const vector<Point>& cc,const Mat& mask);
    Mat drawShapes(Mat img, const vector<Point>& cc);
private:
    vector<Point>hull;
    double x;
    double y;
    double width;
    double height;
    double cmx;
    double cmy;
    QString CurrentView;
};

#endif // COMPUTERVISION_H
