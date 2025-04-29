/* Author: YiWen Lin
   Date: 04/27/2025
   Description: This file creates an inherited class named FilteredPhoto
   that derived class from Photo. It represents a photo after applying 
   artistic filters like Cartoon Sketch, Oil Painting, etc.
*/

#ifndef FILTEREDPHOTO_H
#define FILTEREDPHOTO_H

#include "Photo.h"

class FilteredPhoto : public Photo
{
    private:
        std::string appliedFilter;
        Photo* source; // In order to showImage with origin

    public:
        FilteredPhoto(Photo& original, const std::string& filterName);

        // Overridden virtual functions
        std::string getType() const override;
        void printMetadata() const override;
        void show() const override;

        // FilteredPhoto-specific functions
        void describeFilter() const;

        // Getter
        Photo* getSource() const override;

        // Setter
        void setSource(Photo& original);
        
        // For accessing applied filter
        std::string getAppliedFilter() const;
};

#endif