/* Author: YiWen Lin, Syed Adnan, Poornima Kakade
   Date: 04/05/2025
   Description: 

*/

#ifndef ARTISTICFILTER_H
#define ARTISTICFILTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/xphoto.hpp>
#include <iostream>
using namespace cv;

class ArtisticFilter{
public:
    void pencilSketch(cv::Mat &img);
    void cartoonSketch(cv::Mat &img);
    void oilPainting(cv::Mat &img);
    void popFilming(cv::Mat &img);
    // void oneColorPainting(cv::Mat &img, char color);
    void addDate(Mat &img);
    
};

#endif