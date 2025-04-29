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
