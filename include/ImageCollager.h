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

#include "ImageLoader.h"
#include <opencv2/opencv.hpp>
#include <string>

class ImageCollager : public ImageLoader {
private:
    cv::Mat secondImage;         // second image for collaging
    std::string secondImagePath; // path of the second image

public:
    // Function to load the second image
    bool loadSecondImage(const std::string& path);

    // Function to create the collage
    void createCollage() const;

    // Getter functions for the second image
    const cv::Mat& getSecondImage() const;
    std::string getSecondImagePath() const;
};

#endif // IMAGECOLLAGER_H
