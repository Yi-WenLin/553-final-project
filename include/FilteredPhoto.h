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