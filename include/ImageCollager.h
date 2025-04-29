// Author: Poornima Kakade
// Date: 04/28/2025
// Description: Defines the ImageCollager class, inheriting from Photo.

#ifndef IMAGECOLLAGER_H
#define IMAGECOLLAGER_H

#include "Photo.h"

class ImageCollager : public Photo
{
private:
    cv::Mat secondImage;  // Second image for collage
    std::string secondImagePath; // Optional: to store second image path

public:
    ImageCollager() = default;

    // Load second image
    bool loadSecondImage(const std::string& path);

    // Create collage
    void createCollage() const;

    // Accessors
    const cv::Mat& getSecondImage() const;
    std::string getSecondImagePath() const;
};

#endif // IMAGECOLLAGER_H
