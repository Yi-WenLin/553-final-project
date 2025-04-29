#include "ImageCollager.h"
#include <iostream>

using namespace std;
using namespace cv;

bool ImageCollager::loadSecondImage(const std::string& path)
{
    secondImage = cv::imread(path, IMREAD_COLOR);
    if (secondImage.empty())
    {
        std::cout << "Failed to load second image from: " << path << std::endl;
        return false;
    }
    else
    {
        secondImagePath = path;
        std::cout << "Second image loaded successfully from: " << path << std::endl;
        return true;
    }
}

void ImageCollager::createCollage() const
{
    if (image.empty() || secondImage.empty())
    {
        std::cout << "One or both images are not loaded." << std::endl;
        return;
    }

    // Resize second image if necessary
    cv::Mat resizedSecondImage;
    if (image.size() != secondImage.size())
    {
        cv::resize(secondImage, resizedSecondImage, image.size());
    }
    else
    {
        resizedSecondImage = secondImage;
    }

    // Create collage image
    cv::Mat collage(image.rows, image.cols * 2, image.type());

    // Copy images side-by-side
    image.copyTo(collage(cv::Rect(0, 0, image.cols, image.rows)));
    resizedSecondImage.copyTo(collage(cv::Rect(image.cols, 0, resizedSecondImage.cols, resizedSecondImage.rows)));

    // Show collage
    cv::namedWindow("Collage", cv::WINDOW_AUTOSIZE);
    cv::imshow("Collage", collage);
    std::cout << "Press 'q' or 'ESC' to close the collage window." << std::endl;

    while (true)
    {
        int key = cv::waitKey(10);
        if (key == 27 || key == 'q') // ESC or 'q'
        {
            break;
        }
    }
    cv::destroyAllWindows();
}

const cv::Mat& ImageCollager::getSecondImage() const
{
    return secondImage;
}

std::string ImageCollager::getSecondImagePath() const
{
    return secondImagePath;
}
