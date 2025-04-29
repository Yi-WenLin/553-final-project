/* Author: Poornima Kakade
   Date: 04/28/2025
   Description: This file defines a class named ImageCollager, inheriting from ImageLoader.
*/

#ifndef IMAGECOLLAGER_H
#define IMAGECOLLAGER_H

#include "ImageLoader.h"

class ImageCollager : public ImageLoader {
private:
    cv::Mat secondImage;  // second image for collaging

public:
    void loadSecondImage();            // Load another image
    void createCollage();              // Create collage of two images
};

#endif
