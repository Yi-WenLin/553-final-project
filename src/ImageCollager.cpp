/*
   Author: Poornima Kakade 
   Date: 04/27/2025
   Description: This file is the main control of the system, responsible for loading images, 
                creating a collage from two images, displaying it, and providing the option 
                to save the collage. The system also includes functions for accessing 
                the second image and its path.
*/

#include "ImageCollager.h"
#include <iostream>

using namespace std;
using namespace cv;

// Function to load the second image into the ImageCollager
bool ImageCollager::loadSecondImage(const std::string& path)
{
    // Attempt to load the second image from the given path
    secondImage = cv::imread(path, IMREAD_COLOR);
    
    // Check if the image loaded successfully
    if (secondImage.empty())
    {
        std::cout << "Failed to load second image from: " << path << std::endl;
        return false;  // If the image didn't load, return false
    }
    else
    {
        secondImagePath = path;  // Store the path of the second image
        std::cout << "Second image loaded successfully from: " << path << std::endl;
        return true;  // If the image loaded successfully, return true
    }
}

// Function to create a collage from the first and second images
void ImageCollager::createCollage() const
{
    // Check if both images are loaded
    if (image.empty() || secondImage.empty())
    {
        std::cout << "One or both images are not loaded." << std::endl;
        return;  // If either image is not loaded, exit the function
    }

    // Resize the second image to match the size of the first image if they differ in size
    cv::Mat resizedSecondImage;
    if (image.size() != secondImage.size())
    {
        cv::resize(secondImage, resizedSecondImage, image.size());  // Resize second image
    }
    else
    {
        resizedSecondImage = secondImage;  // No resizing needed if the sizes match
    }

    // Create a new image matrix to hold the collage (side-by-side)
    cv::Mat collage(image.rows, image.cols * 2, image.type());

    // Copy the first image into the left half of the collage
    image.copyTo(collage(cv::Rect(0, 0, image.cols, image.rows)));

    // Copy the resized second image into the right half of the collage
    resizedSecondImage.copyTo(collage(cv::Rect(image.cols, 0, resizedSecondImage.cols, resizedSecondImage.rows)));

    // Add text labels for the two images in the collage: "Original" and "Filtered"
    cv::putText(collage, "Original", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
    cv::putText(collage, "Filtered", cv::Point(image.cols + 10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);

    // Display the created collage in a window
    cv::namedWindow("Collage", cv::WINDOW_AUTOSIZE);
    cv::imshow("Collage", collage);
    std::cout << "Press 'q' or 'ESC' to close the collage window." << std::endl;

    // Ask the user whether they want to save the collage
    std::string savePath;
    std::cout << "Would you like to save the collage? (y/n): ";
    char choice;
    std::cin >> choice;  // Get user's choice

    // If user chooses to save, ask for the path and save the collage
    if (choice == 'y' || choice == 'Y') {
        std::cout << "Enter the path to save the collage (e.g., collage.jpg): ";
        std::cin >> savePath;
        cv::imwrite(savePath, collage);  // Save the collage as an image file
        std::cout << "Collage saved to " << savePath << std::endl;
    }

    // Wait for the user to press 'q' or 'ESC' to close the window
    while (true)
    {
        int key = cv::waitKey(10);
        if (key == 27 || key == 'q') break;  // ESC or 'q' to close
    }
    cv::destroyAllWindows();  // Close the OpenCV window
}

// Function to get the second image as a constant reference
const cv::Mat& ImageCollager::getSecondImage() const
{
    return secondImage;  // Return the second image
}

// Function to get the path of the second image
std::string ImageCollager::getSecondImagePath() const
{
    return secondImagePath;  // Return the path of the second image
}
