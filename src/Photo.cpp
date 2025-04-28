#include "Photo.h"
#include <iostream>
#include <algorithm> // for std::find
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

bool Photo::load(const std::string& path)
{
    image = cv::imread(path);
    // Empty Case
    if(image.empty())
    {
        std::cerr << "Failed to load image from: " << path << std::endl;
        return false;
    }

    this->path = path;
    // addTag("Loaded"); // Instead, using original to mark
    return true;
}

bool Photo::save(const std::string& path)
{
    // Empty Case
    if(image.empty())
    {
        std::cerr << "No image data to save. " << std::endl;
        return false;
    }

    // Write out
    if(cv::imwrite(path, image))
    {
        std::cout << "Saved image to: " << path << std::endl;
        return true;
    }

    // When error
    std::cerr << "Failed to save image.\n";
    return false;
}

void Photo::show() const
{
    // Empty Case
    if(image.empty())
    {
        std::cerr << "No image to display. " << std::endl;
        return;
    }
    
    cv::imshow("Photo Viewer", image);
    std::cout << "Press '0' to close the image window." << std::endl;
    cv::waitKey(0);
    cv::destroyWindow("Photo Viewer");
}

void Photo::setOriginal(const std::string& original)
{
    this->original = original;
}

void Photo::addTag(const std::string& tag)
{
    // Check repetition
    if(!hasTag(tag))
    {
        tags.push_back(tag);
    }
}

bool Photo::hasTag(const std::string& tag) const
{
    // False if vector iterator reach .end()
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

const std::vector<std::string>& Photo::getTags() const
{
    return tags;
}

void Photo::setName(const std::string& name)
{
    this->name = name;
}

std::string Photo::getName() const
{
    return name;
}

void Photo::setPath(const std::string& path)
{
    this->path = path;
}

std::string Photo::getPath() const
{
    return path;
}

cv::Mat& Photo::getImage()
{
    return image;
}

std::string Photo::getType() const
{
    return "Photo";
}

void Photo::printMetadata() const
{
    std::cout << "---------- Metadata ----------\n";
    std::cout << "Type: " << getType() << std::endl;
    std::cout << "Original/Derivative: " << original << std::endl;
    std::cout << "Tags: ";
    for (const auto& tag : tags)
        std::cout << tag << ", ";
    std::cout << "\n";
    if(!image.empty())
        std::cout << "Resolution: " << image.cols << " x " << image.rows << std::endl;
    else
        std::cout << "No image. " << std::endl;
}