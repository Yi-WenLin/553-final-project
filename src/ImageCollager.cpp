/*
   Author: Poornima Kakade 
   Date: 04/27/2025
   Description: This file is the main control of the system, responsible for loading images, 
                creating a collage from two images, displaying it, and providing the option 
                to save the collage. The system also includes functions for accessing 
                the second image and its path.
*/

#include "ImageCollager.h"
#include <opencv2/opencv.hpp>
#include <iostream>

ImageCollager::ImageCollager() : Photo()
{
    // Initialize the images as empty
    image1 = cv::Mat();
    image2 = cv::Mat();
}

ImageCollager::~ImageCollager()
{
    // Destructor can clean up resources if needed (OpenCV handles it automatically)
}

void ImageCollager::loadImage(const cv::Mat& img)
{
    image1 = img;
}

void ImageCollager::loadSecondImage(const cv::Mat& img)
{
    image2 = img;
}

void ImageCollager::createCollage()
{
    if (image1.empty() || image2.empty())
    {
        std::cout << "Both images must be loaded before creating a collage!" << std::endl;
        return;
    }

    // Resize the images to be of equal size for the collage
    int width = std::max(image1.cols, image2.cols);
    int height = std::max(image1.rows, image2.rows);

    cv::Mat resizedImage1, resizedImage2;
    cv::resize(image1, resizedImage1, cv::Size(width, height));
    cv::resize(image2, resizedImage2, cv::Size(width, height));

    // Create a new canvas to hold the collage
    cv::Mat collage(height, width * 2, CV_8UC3);

    // Place both images on the canvas (side by side)
    resizedImage1.copyTo(collage(cv::Rect(0, 0, width, height)));
    resizedImage2.copyTo(collage(cv::Rect(width, 0, width, height)));

    // Store the result as the collage image
    image1 = collage;  // The collage itself is now the first image
}

void ImageCollager::show() const
{
    if (image1.empty())
    {
        std::cout << "No collage to display!" << std::endl;
        return;
    }

    // Display the collage using OpenCV
    cv::imshow("Collage", image1);
    cv::waitKey(0);  // Wait until a key is pressed
}

void ImageCollager::save(const std::string& path) const
{
    if (image1.empty())
    {
        std::cout << "No collage to save!" << std::endl;
        return;
    }

    // Save the collage to the specified path
    if (!cv::imwrite(path, image1))
    {
        std::cout << "Failed to save the collage!" << std::endl;
    }
    else
    {
        std::cout << "Collage saved successfully at: " << path << std::endl;
    }
}
