#ifndef MANUALCOMPUTERVISIONSUBTRACTION_H
#define MANUALCOMPUTERVISIONSUBTRACTION_H


#include <opencv2/opencv.hpp>
#include <QFile>
using namespace cv;
using namespace std;
class ComputerVisionSub
{
public:
    Mat remove_backgroundSub(const Mat& img, const Mat& blank, int blurKM,int tLowM,int tHighM,int b1LM,
                          int b1HM,int b2LM,int b2HM,int x1, int y1, int x2, int y2,bool ColorStandardization,
                          int BluePlantThreshold, int BluePlantBlur, int GreenPlantThreshold, int GreenPlantBlur, int MaskAlphaThresholdDark,
                          int MaskAlphaThresholdLight,int MaskBetaThreshold,int DifferenceDilateKernelSize, int DifferenceErodeKernelSize,int PotDilateKernelSize, int PotErodeKernelSize);
    Mat get_RGB_HISTSub(const Mat& img, const Mat& mask);
    Mat get_color_RSub(const Mat& img, const Mat& mask);
    Mat get_color_GSub(const Mat& img, const Mat& mask);
    Mat get_color_BSub(const Mat& img, const Mat& mask);
    Mat get_color_LSub(const Mat& img, const Mat& mask);
    Mat get_color_GMSub(const Mat& img, const Mat& mask);
    Mat get_color_BYSub(const Mat& img, const Mat& mask);
    Mat get_color_HueSub(const Mat& img, const Mat& mask);
    Mat get_color_SaturationSub(const Mat& img, const Mat& mask);
    Mat get_color_ValueSub(const Mat& img, const Mat& mask);
    vector<int> getNodesSub(const Mat& img);
    vector<int> getTipsSub(const Mat& img);

    vector<Point> get_ccSub(Mat img, int x1, int y1, int x2, int y2);
    vector<double> get_shapesSub(const vector<Point>& cc,const Mat& mask);
    Mat drawShapesSub(Mat img, const vector<Point>& cc);
private:
    vector<Point>hull;
    double x;
    double y;
    double width;
    double height;
    double cmx;
    double cmy;
};
#endif // MANUALCOMPUTERVISIONSUBTRACTION_H
