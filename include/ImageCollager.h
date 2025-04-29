/*
   Author: Poornima Kakade 
   Date: 04/27/2025
   Description: This header file defines the ImageCollager class, which inherits from the 
                ImageLoader class. The ImageCollager class is responsible for loading a 
                second image, creating a collage by combining the original and second images, 
                and providing getter functions for accessing the second image and its path.
                It includes function declarations for loading the second image, creating the 
                collage, and retrieving the second image and its path.
*/

#ifndef IMAGECOLLAGER_H
#define IMAGECOLLAGER_H

#include "Photo.h"
#include <opencv2/opencv.hpp>

class ImageCollager : public Photo
{
private:
    cv::Mat image1;  // The first image (original)
    cv::Mat image2;  // The second image (filtered or transferred)

public:
    ImageCollager();
    ~ImageCollager();

    // Loads the first image (original photo)
    void loadImage(const cv::Mat& img);

    // Loads the second image (filtered or transferred photo)
    void loadSecondImage(const cv::Mat& img);

    // Creates the collage by combining both images
    void createCollage();

    // Shows the collage using OpenCV
    void show() const override;

    // Save the collage to a specified path
    void save(const std::string& path) const override;
};

#endif // IMAGECOLLAGER_H
