/* Author: YiWen Lin, Syed Adnan, Poornima Kakade
   Date: 04/05/2025
   Description: 

*/

#include "ImageLoader.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void ImageLoader::loadImage(){
    std::string path;
    std::cout << "Enter image path: ";
    //std::cin >> path;
    cin.ignore();
    getline(cin, path);
    // path = "/Users/carollyn/Desktop/EE 553 C++/Milestone/input/test_img.jpg";
    image = cv::imread(path, IMREAD_COLOR);
    if (image.empty()) {
        std::cout << "Image load failed." << endl;
    } else {
        std::cout << "Image loaded successfully." << endl;
    }
}

void ImageLoader::showImage(){
    if (image.empty()){
        cout << "No image loaded." << endl;
        return;
    }
    cv::namedWindow("Processed Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Processed Image", image);
    std::cout << "Press 'q' or 'ESC' to close the image window." << std::endl;

    // One things need to check with the OS
    // MacOS seems to have some problem when running the show image
    // OpenCV windows crashed but program works well
    while (true) {
        int key = cv::waitKey(10);  // wait 10ms and allow UI events
        if (key == 27 || key == 'q') {  // ESC or 'q'
            break;
        }
    }
    cv::destroyAllWindows();  // close all OpenCV windows
}

void ImageLoader::saveImage(){
    std::string filename;
    std::cout << "Enter filename to save (without .jpg suffix): ";
    cin.ignore();
    getline(cin, filename);
    std::string path = "/Users/carollyn/Desktop/EE 553 C++/Milestone/output/" + filename + ".jpg";
    cv::imwrite(path, image);
}

cv::Mat& ImageLoader::getImage(){
    return image;
}