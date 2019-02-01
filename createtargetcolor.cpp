#include "createtargetcolor.h"

#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <QSettings>
#include <QDir>
#include <QTextStream>
using namespace cv;
using namespace std;

float get_color(Mat img,Mat &mask){
        Mat img1 = img;
    Mat hist;
        int dims = 1;
        int histSize = 255;
        float hranges[] = { 0, 255 };
        const float *ranges = {hranges};

        calcHist(&img1,1,0,mask,hist, dims, &histSize, &ranges ,true ,false);

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

void createTargetColor::ColorFound(cv::Mat cc){

  QSettings internal("internal.ini",QSettings::IniFormat);
  QString ProjectDirA = internal.value("ProjectDir").value<QString>();
  QString ProjectName = internal.value("ProjectName").value<QString>();
  QSettings setup(ProjectDirA,QSettings::IniFormat);
  QString ProjectDir = setup.value("ProjectDir").value<QString>();

  QString CurrentView = internal.value("CurrentView").value<QString>();

  QString Path;

  QString fileDir = ProjectDir+"/ColorMasks/";
  if(!QDir(fileDir).exists()){
      QDir().mkdir(fileDir);
    }
  QString temp;
  temp = fileDir+CurrentView;
  QFile data(fileDir+CurrentView+"target.csv");

    vector<Mat> bgr;
    split(cc, bgr);
    Mat b = bgr[0];
    Mat g = bgr[1];
    Mat r = bgr[2];
        for(unsigned int i=1;i<23;i++){
                stringstream ss;
                ss << i;
                string str = ss.str();
                string file_name = temp.toUtf8().constData()+str+".jpg";
                Mat mask = imread(file_name,0);
                Mat cc;
                        threshold(mask,cc,90,255,THRESH_BINARY);

                float b_avg = get_color(b, cc);
                float g_avg = get_color(g, cc);
                float r_avg = get_color(r, cc);

                if(data.open(QFile::WriteOnly | QIODevice::Append)){
                    QTextStream out(&data);
                    out << b_avg << ","<< g_avg <<","<<r_avg <<endl;
                    data.close();
                  }
        }
}
