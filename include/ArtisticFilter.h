/* Author: YiWen Lin
   Date: 04/27/2025
   Description: This file defines a class named ArtisticFilter for 
   easier managing all artistic filters when user want to apply them.
*/

#ifndef ARTISTICFILTER_H
#define ARTISTICFILTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/xphoto.hpp>
#include <string>

class ArtisticFilter
{
public:
    // void pencilSketch(cv::Mat& img) const;
    void cartoonSketch(cv::Mat& img) const;
    void oilPainting(cv::Mat& img) const;
    void popFilming(cv::Mat& img) const;
    // void oneColorPainting(cv::Mat& img, char color);
    void addDate(cv::Mat& img) const;
};

#endif