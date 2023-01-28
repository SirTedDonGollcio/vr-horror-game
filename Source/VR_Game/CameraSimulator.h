#pragma once

#include <iostream>
#include <stdio.h>
//#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"    
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;
using namespace std;

class EventCamera
{
public:
    Mat frame1_t, frame2_t;
    VideoCapture* camera;
    int theFirstTime = 0;
    Mat BlackScreen, sum1, sum2, difference;


    Mat getDiff_andReconstruction(Mat frame);
};