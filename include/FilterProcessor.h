// FilterProcessor.h - For applying various filters
#ifndef FILTER_PROCESSOR_H
#define FILTER_PROCESSOR_H

#include "ImageProcessor.h"

class FilterProcessor : public ImageProcessor {
private:
    int filterType;
    int kernelSize;

public:
    // Constants for filter types
    static const int FILTER_BLUR = 0;
    static const int FILTER_GAUSSIAN = 1;
    static const int FILTER_MEDIAN = 2;
    static const int FILTER_BILATERAL = 3;
    
    // Constructor
    FilterProcessor(int type = FILTER_GAUSSIAN, int ksize = 5);
    
    // Override the process function
    virtual bool processImage() override;
    
    // Set filter parameters
    void setFilterType(int type);
    void setKernelSize(int size);
};

#endif // FILTER_PROCESSOR_H