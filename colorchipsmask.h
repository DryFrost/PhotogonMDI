#ifndef COLORCHIPSMASK_H
#define COLORCHIPSMASK_H


#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/opencv.hpp>

#define MACBETH_WIDTH   6
#define MACBETH_HEIGHT  4
#define MACBETH_SQUARES MACBETH_WIDTH * MACBETH_HEIGHT

#define MAX_CONTOUR_APPROX  7

#define MAX_RGB_DISTANCE 444

class colorChipsMask
{
public:
  IplImage * find_macbeth1( IplImage *macbeth_img );
};

#endif // COLORCHIPSMASK_H
