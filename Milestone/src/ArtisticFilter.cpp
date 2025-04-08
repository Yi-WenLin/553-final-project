/* Author: YiWen Lin
   Date: 04/05/2025
   Description: This file provides the definition of the member function of 
   ArtisticFilter class.
*/

#include "ArtisticFilter.h"
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

void ArtisticFilter::pencilSketch(Mat &img){
    // Convert to grayscale
    cv::Mat gray, inverted;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::bitwise_not(gray, inverted);

    // Blur the image using Gaussian Blur
    cv::Mat blurred;
    cv::GaussianBlur(inverted, blurred, cv::Size(21, 21), 0);

    // Blend
    cv::Mat sketch;
    cv::divide(gray, 255 - blurred, sketch, 256.0);
    img = sketch;

    cout << "Applied Pencil Sketch." << endl;
}

void ArtisticFilter::cartoonSketch(Mat &img){
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

    // Resize to smaller size (more faster bilateral filtering)
    cv::Size smallSize(img.cols / 2, img.rows / 2);
    cv::Mat smallImg;
    cv::resize(img, smallImg, smallSize, 0, 0, cv::INTER_LINEAR);
    cv::Mat tmp = Mat(smallSize, CV_8UC3);

    // Repeat for stronger cartoon effect
    int repetitions = 7;
    int fileter_size = 9; // large effect on speed
    double sigmaColor = 9; // filter color strength
    double sigmaSpace = 7; // spatial strength - affect speed
    for (int i = 0; i < repetitions; i++) {
        cv::bilateralFilter(smallImg, tmp, fileter_size, sigmaColor, sigmaSpace);
        cv::bilateralFilter(tmp, smallImg, fileter_size, sigmaColor, sigmaSpace);
    }

    // Resize back to original size
    cv::Mat bigImg;
    cv::resize(smallImg, bigImg, img.size(), 0, 0, cv::INTER_LINEAR);

    // Edge mask
    cv::Mat cartoon = cv::Mat::zeros(img.size(), CV_8UC3);
    bigImg.copyTo(cartoon, mask);
    img = cartoon;

    cout << "Applied Cartoon Sketch." << endl;
}

void ArtisticFilter::oilPainting(Mat &img){
    cv::xphoto::oilPainting(img, img, 7, 1); // size of neighborhood | number of bins
    cout << "Applied Oil Painting." << endl;
}

void ArtisticFilter::popFilming(Mat &img){
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
    hsvChannels[1] = cv::min(hsvChannels[1] * 1.25, 1.0f); // saturation
    cv::merge(hsvChannels, hsv);
    cv::cvtColor(hsv, in, cv::COLOR_HSV2BGR);

    // Boost brightness and contrast
    in = (in - 0.4) * 1.4 + 0.4; // more aggressive than original
    cv::min(cv::max(in, 0.0f), 1.0f, in); // clamp
    in.convertTo(img, CV_8UC3, 255.0);
    
    cout << "Applied Edge Painting." << endl;
}

void ArtisticFilter::addDate(Mat &img){
    // Implemented by drawing the border first, then covering a the main text
    time_t now = std::time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "\'\' %m %d %y", std::localtime(&now)); // "''25 04 06"
    string dateText(buf);

    int font = cv::FONT_HERSHEY_PLAIN; // font style
    double fontScale = 6; // font size
    int thicknessBorder = 5; // border thickness
    int thicknessText = 2;   // main text thickness

    cv::Scalar borderColor(0, 128, 255); // Orange border (BGR) - white: (255, 255, 255)
    cv::Scalar textColor(0, 183, 255); // Yellow main text

    int baseline = 0;
    cv::Size textSize = cv::getTextSize(dateText, font, fontScale, thicknessBorder, &baseline);

    // Position - right bottom with 10px padding
    cv::Point textOrg(img.cols - textSize.width - 10, img.rows - 10);

    // Draw border
    cv::putText(img, dateText, textOrg, font, fontScale, borderColor, thicknessBorder, cv::LINE_AA);
    // Draw main text
    cv::putText(img, dateText, textOrg, font, fontScale, textColor, thicknessText, cv::LINE_AA);

    cout << "Date Added." << endl;
}
