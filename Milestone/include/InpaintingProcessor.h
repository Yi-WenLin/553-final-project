// InpaintingProcessor.h - Derived class for inpainting operations
#ifndef INPAINTING_PROCESSOR_H
#define INPAINTING_PROCESSOR_H

#include "ImageProcessor.h"

class InpaintingProcessor : public ImageProcessor {
private:
    cv::Mat maskImage;
    int inpaintingMethod;
    double inpaintingRadius;

public:
    // Constants for inpainting methods
    static const int INPAINT_NS = cv::INPAINT_NS;
    static const int INPAINT_TELEA = cv::INPAINT_TELEA;
    
    // Constructor
    InpaintingProcessor(int method = INPAINT_NS, double radius = 3.0);
    
    // Override the process function
    virtual bool processImage() override;
    
    // Create a mask from user input (this could be implemented in various ways)
    bool createMaskFromUserInput();
    
    // Load a pre-created mask
    bool loadMask(const std::string& filename);
    
    // Set inpainting parameters
    void setMethod(int method);
    void setRadius(double radius);
};

#endif // INPAINTING_PROCESSOR_H