/* Author: YiWen Lin
   Date: 04/27/2025
   Description: This file provides the defination of the ArtisticFilter class.
*/

#include "ArtisticFilter.h"
#include <opencv2/imgproc.hpp> //resize, text
#include <opencv2/photo.hpp>
#include <opencv2/xphoto.hpp> // oilPainting
#include <ctime>
#include <string>
#include <iomanip>

using namespace cv;

/*
void ArtisticFilter::pencilSketch(cv::Mat& img)
{
    cv::Mat gray, inverted, blurred, sketch;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::bitwise_not(gray, inverted);
    cv::GaussianBlur(inverted, blurred, cv::Size(21, 21), 0);
    cv::divide(gray, 255 - blurred, sketch, 256.0);
    cv::cvtColor(sketch, img, cv::COLOR_GRAY2BGR);
}
*/

void ArtisticFilter::cartoonSketch(Mat &img) const
{
    // Will first resize to smaller size for faster bilateral filtering
    // Then resize back to origin

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // Apply median blur
    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    cv::medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

    // Detect edges using Laplacian
    cv::Mat edges;
    const int LAPLACIAN_FILTER_SIZE = 5;
    cv::Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);

    // Binarize edges to create a mask (sketch effect)
    cv::Mat mask;
    const int EDGES_THRESHOLD = 80;
    cv::threshold(edges, mask, EDGES_THRESHOLD, 255, cv::THRESH_BINARY_INV);

    // Resize to smaller size (faster bilateral filtering)
    cv::Size smallSize(img.cols / 2, img.rows / 2);
    cv::Mat smallImg;
    cv::resize(img, smallImg, smallSize, 0, 0, cv::INTER_LINEAR);
    cv::Mat tmp = Mat(smallSize, CV_8UC3);

    // Repeat for stronger cartoon effect
    int repetitions = 7;
    int fileter_size = 9; // Large effect on speed
    double sigmaColor = 9; // Filter color strength.
    double sigmaSpace = 7; // Spatial strength - Affects speed.
    for(int i = 0; i < repetitions; i++)
    {
        cv::bilateralFilter(smallImg, tmp, fileter_size, sigmaColor, sigmaSpace);
        cv::bilateralFilter(tmp, smallImg, fileter_size, sigmaColor, sigmaSpace);
    }

    // Resize back to original size
    cv::Mat bigImg;
    cv::resize(smallImg, bigImg, img.size(), 0, 0, cv::INTER_LINEAR);

    // Apply edge mask
    cv::Mat cartoon = cv::Mat::zeros(img.size(), CV_8UC3);
    bigImg.copyTo(cartoon, mask);
    img = cartoon;

    std::cout << "Applied Cartoon Sketch. " << std::endl;
}

void ArtisticFilter::oilPainting(Mat &img) const
{
    cv::xphoto::oilPainting(img, img, 7, 1); // size of neighborhood | number of bins
    std::cout << "Applied Oil Painting. " << std::endl;
}

void ArtisticFilter::popFilming(Mat &img) const
{
    // Trying to creat a fujifilm-like with pop style - will modify
    
    // Normalize to [0,1]
    cv::Mat in;
    img.convertTo(in, CV_32FC3, 1.0 / 255.0);

    std::vector<cv::Mat> channels(3);
    cv::split(in, channels);

    // Boost red and blue channel
    channels[2] = cv::min(channels[2] * 1.08, 1.0f); // red
    channels[0] = cv::min(channels[0] * 1.05, 1.0f); // blue

    // Add punch to saturation (HSV-style approximation)
    cv::Mat hsv;
    cv::cvtColor(in, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> hsvChannels(3);
    cv::split(hsv, hsvChannels);
    hsvChannels[1] = cv::min(hsvChannels[1] * 1.25, 1.0f); // boost saturation
    cv::merge(hsvChannels, hsv);
    cv::cvtColor(hsv, in, cv::COLOR_HSV2BGR);

    // Boost brightness and contrast
    in = (in - 0.4) * 1.4 + 0.4; // more aggressive than original
    // cv::min(cv::max(in, 0.0f), 1.0f, in); // clamp
    cv::max(in, 0.0f, in);
    cv::min(in, 1.0f, in);

    in.convertTo(img, CV_8UC3, 255.0);
    
    std::cout << "Applied Edge Painting." << std::endl;
}

void ArtisticFilter::addDate(Mat &img) const
{
    // Implemented by drawing the border first, then covering a the main text
    
    time_t now = std::time(0);

    char buf[80];
    strftime(buf, sizeof(buf), "\'\' %m %d %y", std::localtime(&now)); // "''25 04 06" Local Time
    std::string dateText(buf);

    int font = cv::FONT_HERSHEY_PLAIN; // font style

    // Version 2 - How to adjust the size of font by the size of the image
    double fontScale = max(img.cols, img.rows) / 600.0; // Adjustable font size
    int thicknessBorder = 3; // border thickness 5
    int thicknessText = 1;   // main text thickness 5

    cv::Scalar borderColor(0, 128, 255); // Orange border (BGR) - white: (255, 255, 255)
    cv::Scalar textColor(0, 183, 255); // Yellow main text

    int baseline = 0;
    cv::Size textSize = cv::getTextSize(dateText, font, fontScale, thicknessBorder, &baseline);

    // Position - right bottom with 10px padding
    cv::Point textOrg(img.cols - textSize.width - 20, img.rows - 20); // 10

    // Draw border
    cv::putText(img, dateText, textOrg, font, fontScale, borderColor, thicknessBorder, cv::LINE_AA);
    // Draw main text
    cv::putText(img, dateText, textOrg, font, fontScale, textColor, thicknessText, cv::LINE_AA);

    std::cout << "Date Added. " << std::endl;
}