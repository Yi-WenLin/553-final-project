#include "ImageCollager.h"
#include <iostream>

using namespace std;
using namespace cv;

void ImageCollager::loadSecondImage() {
    std::string path;
    std::cout << "Enter second image path: ";
    cin.ignore();
    getline(cin, path);

    secondImage = cv::imread(path, IMREAD_COLOR);
    if (secondImage.empty()) {
        std::cout << "Second image load failed." << endl;
    }
    else {
        std::cout << "Second image loaded successfully." << endl;
    }
}

void ImageCollager::createCollage() {
    if (getImage().empty() || secondImage.empty()) {
        std::cout << "One or both images are not loaded." << endl;
        return;
    }

    // Resize second image to match first image size (if needed)
    if (getImage().size() != secondImage.size()) {
        cv::resize(secondImage, secondImage, getImage().size());
    }

    // Create a new Mat to hold the collage (width = sum of two widths)
    cv::Mat collage(getImage().rows, getImage().cols * 2, getImage().type());

    // Copy the first and second images side by side
    getImage().copyTo(collage(cv::Rect(0, 0, getImage().cols, getImage().rows)));
    secondImage.copyTo(collage(cv::Rect(getImage().cols, 0, secondImage.cols, secondImage.rows)));

    // Show the collage
    cv::namedWindow("Collage", cv::WINDOW_AUTOSIZE);
    cv::imshow("Collage", collage);
    std::cout << "Press 'q' or 'ESC' to close the collage window." << std::endl;

    while (true) {
        int key = cv::waitKey(10);
        if (key == 27 || key == 'q') { 
            break;
        }
    }
    cv::destroyAllWindows();
}
