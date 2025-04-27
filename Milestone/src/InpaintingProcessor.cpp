// InpaintingProcessor.cpp
#include "InpaintingProcessor.h"
#include <iostream>

InpaintingProcessor::InpaintingProcessor(int method, double radius) 
    : ImageProcessor("Inpainting"), inpaintingMethod(method), inpaintingRadius(radius) {
    // Constructor implementation
    std::cout << "Inpainting parameters: method=" << method << ", radius=" << radius << std::endl;
}

bool InpaintingProcessor::processImage() {
    if (originalImage.empty()) {
        std::cerr << "Error: No original image loaded" << std::endl;
        return false;
    }
    
    if (maskImage.empty()) {
        std::cerr << "Error: No mask image defined for inpainting" << std::endl;
        return false;
    }
    
    try {
        // Make sure mask is grayscale
        cv::Mat grayMask;
        if (maskImage.channels() > 1) {
            cv::cvtColor(maskImage, grayMask, cv::COLOR_BGR2GRAY);
        } else {
            grayMask = maskImage.clone();
        }
        
        // Apply inpainting
        cv::inpaint(originalImage, grayMask, processedImage, inpaintingRadius, inpaintingMethod);
        
        std::cout << "Inpainting completed successfully" << std::endl;
        return true;
    }
    catch (const cv::Exception& e) {
        std::cerr << "Exception during inpainting: " << e.what() << std::endl;
        return false;
    }
}

bool InpaintingProcessor::loadMask(const std::string& filename) {
    try {
        maskImage = cv::imread(filename, cv::IMREAD_GRAYSCALE);
        
        if (maskImage.empty()) {
            std::cerr << "Error: Could not load mask image " << filename << std::endl;
            return false;
        }
        
        // Ensure mask has same dimensions as original image
        if (!originalImage.empty() && 
            (maskImage.rows != originalImage.rows || maskImage.cols != originalImage.cols)) {
            std::cerr << "Warning: Mask dimensions don't match image. Resizing mask." << std::endl;
            cv::resize(maskImage, maskImage, originalImage.size());
        }
        
        return true;
    }
    catch (const cv::Exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return false;
    }
}

bool InpaintingProcessor::createMaskFromUserInput() {
    if (originalImage.empty()) {
        std::cerr << "Error: No original image loaded" << std::endl;
        return false;
    }
    
    // Create a black mask of the same size as the original image
    maskImage = cv::Mat::zeros(originalImage.size(), CV_8UC1);
    
    // Create a copy of the original image for drawing
    cv::Mat tempImage = originalImage.clone();
    
    // Named window
    std::string windowName = "Create Mask: Left-click and drag to mark areas for inpainting";
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    
    // Variables for mouse callback
    struct MouseData {
        cv::Mat& mask;
        cv::Mat& image;
        bool drawing;
        cv::Point lastPoint;
        
        MouseData(cv::Mat& m, cv::Mat& i) : mask(m), image(i), drawing(false) {}
    };
    
    MouseData data(maskImage, tempImage);
    
    // Mouse callback function
    auto mouseCallback = [](int event, int x, int y, int flags, void* userdata) {
        MouseData* data = reinterpret_cast<MouseData*>(userdata);
        
        if (event == cv::EVENT_LBUTTONDOWN) {
            data->drawing = true;
            data->lastPoint = cv::Point(x, y);
        }
        else if (event == cv::EVENT_MOUSEMOVE && data->drawing) {
            // Draw white line on mask
            cv::line(data->mask, data->lastPoint, cv::Point(x, y), cv::Scalar(255), 5);
            
            // Draw red line on temp image to visualize
            cv::line(data->image, data->lastPoint, cv::Point(x, y), cv::Scalar(0, 0, 255), 5);
            
            data->lastPoint = cv::Point(x, y);
        }
        else if (event == cv::EVENT_LBUTTONUP) {
            data->drawing = false;
        }
    };
    
    cv::setMouseCallback(windowName, mouseCallback, &data);
    
    std::cout << "Creating mask: Left-click and drag to mark areas for inpainting." << std::endl;
    std::cout << "Press 'c' to clear the mask, 'r' to reset, or 'Enter' to finish." << std::endl;
    
    while (true) {
        cv::imshow(windowName, tempImage);
        int key = cv::waitKey(1);
        
        if (key == 13) { // Enter key
            break;
        }
        else if (key == 'c') {
            // Clear mask
            maskImage = cv::Mat::zeros(originalImage.size(), CV_8UC1);
            tempImage = originalImage.clone();
        }
        else if (key == 'r') {
            // Reset
            return createMaskFromUserInput();
        }
    }
    
    cv::destroyWindow(windowName);
    return true;
}

void InpaintingProcessor::setMethod(int method) {
    if (method != INPAINT_NS && method != INPAINT_TELEA) {
        std::cerr << "Warning: Invalid inpainting method. Using default (INPAINT_NS)." << std::endl;
        inpaintingMethod = INPAINT_NS;
    } else {
        inpaintingMethod = method;
    }
}

void InpaintingProcessor::setRadius(double radius) {
    if (radius <= 0) {
        std::cerr << "Warning: Radius must be positive. Using default (3.0)." << std::endl;
        inpaintingRadius = 3.0;
    } else {
        inpaintingRadius = radius;
    }
}