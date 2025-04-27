// FilterProcessor.cpp
#include "FilterProcessor.h"
#include <iostream>

FilterProcessor::FilterProcessor(int type, int ksize) 
    : ImageProcessor("Filter"), filterType(type), kernelSize(ksize) {
    // Constructor implementation
    std::cout << "Filter parameters: type=" << type << ", kernel size=" << ksize << std::endl;
}

bool FilterProcessor::processImage() {
    if (originalImage.empty()) {
        std::cerr << "Error: No original image loaded" << std::endl;
        return false;
    }
    
    try {
        switch (filterType) {
            case FILTER_BLUR:
                cv::blur(originalImage, processedImage, cv::Size(kernelSize, kernelSize));
                std::cout << "Applied box blur filter" << std::endl;
                break;
                
            case FILTER_GAUSSIAN:
                cv::GaussianBlur(originalImage, processedImage, 
                                cv::Size(kernelSize, kernelSize), 0, 0);
                std::cout << "Applied Gaussian blur filter" << std::endl;
                break;
                
            case FILTER_MEDIAN:
                cv::medianBlur(originalImage, processedImage, kernelSize);
                std::cout << "Applied median blur filter" << std::endl;
                break;
                
            case FILTER_BILATERAL:
                cv::bilateralFilter(originalImage, processedImage, kernelSize, 
                                  kernelSize*2, kernelSize/2);
                std::cout << "Applied bilateral filter" << std::endl;
                break;
                
            default:
                std::cerr << "Error: Unknown filter type" << std::endl;
                return false;
        }
        
        return true;
    }
    catch (const cv::Exception& e) {
        std::cerr << "Exception during filtering: " << e.what() << std::endl;
        return false;
    }
}

void FilterProcessor::setFilterType(int type) {
    if (type < FILTER_BLUR || type > FILTER_BILATERAL) {
        std::cerr << "Warning: Invalid filter type. Using default (Gaussian)." << std::endl;
        filterType = FILTER_GAUSSIAN;
    } else {
        filterType = type;
    }
}

void FilterProcessor::setKernelSize(int size) {
    // Kernel size must be positive and odd
    if (size <= 0 || size % 2 == 0) {
        std::cerr << "Warning: Kernel size must be positive and odd. Using default (5)." << std::endl;
        kernelSize = 5;
    } else {
        kernelSize = size;
    }
}