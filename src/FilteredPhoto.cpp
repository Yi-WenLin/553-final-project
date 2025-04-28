#include "Photo.h"
#include "FilteredPhoto.h"
#include "TransferredPhoto.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

FilteredPhoto::FilteredPhoto(Photo& original, const std::string& filterName)
    : appliedFilter(filterName), source(nullptr)    // Pre-set
{
    this->image = original.getImage().clone();  // Copy image
    this->tags = original.getTags();  // Copy path
    addTag(filterName);  // Add filterName
    this->original = "Derivative";
}

Photo* FilteredPhoto::getSource() const
{
    return source;
}

void FilteredPhoto::setSource(Photo& original)
{
    // If passin is a processed image, will track the same source.
    if(FilteredPhoto* temp = dynamic_cast<FilteredPhoto*>(&original))
    {
        this->source = temp->getSource();
    }
    // If transferred image
    else if(TransferredPhoto* temp = dynamic_cast<TransferredPhoto*>(&original))
    {
        this->source = temp->getSource();
    }
    // Pass in normal image
    else
    {
        this->source = &original;
    }

}

std::string FilteredPhoto::getType() const
{
    return "FilteredPhoto";
}

void FilteredPhoto::describeFilter() const
{
    std::cout << "Filter applied: " << appliedFilter << endl;
}


void FilteredPhoto::printMetadata() const
{
    Photo::printMetadata();
    describeFilter();
}

void FilteredPhoto::show() const
{
    // Rule: Left for original image, Right for current image

    // Empty Case
    if(!source)
    {
        std::cerr << "No source image availability." << std::endl;
        return;
    }

    // Empty Case
    cv::Mat left = source->getImage();
    if(left.empty() || image.empty())
    {
        std::cout << "One of the images is empty." << std::endl;
        return;
    }

    // Resize to make sure they have the same height - if needed
    cv::Mat right = image;
    if (left.rows != right.rows)
    {
        double scale = static_cast<double>(left.rows) / right.rows;
        cv::resize(image, right, cv::Size(static_cast<int>(image.cols * scale), image.rows));
    }

    // Create a separator - white line
    int thickness = 3; // thickness
    cv::Mat separator(left.rows, thickness, left.type(), cv::Scalar(255, 255, 255)); // white

    // Concatenate: [ORIGINAL | Separator | CURRENT]
    std::vector<cv::Mat> images_to_concat = { left, separator, image };
    cv::Mat combined;
    cv::hconcat(images_to_concat, combined);

    cv::imshow("Filtered Photo Viewer", combined);
    std::cout << "Press '0' to close the image window." << std::endl;
    cv::waitKey(0);
    cv::destroyWindow("Filtered Photo Viewer");
}

std::string FilteredPhoto::getAppliedFilter() const
{
    return appliedFilter;
}
