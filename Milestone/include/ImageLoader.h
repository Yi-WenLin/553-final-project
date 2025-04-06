/* Author: YiWen Lin, Syed Adnan, Poornima Kakade
   Date: 04/05/2025
   Description: 

*/

#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include <opencv2/opencv.hpp>

class ImageLoader{
private:
    cv::Mat image;
    
public:
    void loadImage();
    void showImage();
    void saveImage();

    cv::Mat& getImage();  // Getter to access image reference

};

#endif