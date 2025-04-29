/* Author: YiWen Lin
   Date: 04/27/2025
   Description: This file creates a base class named Photo for all types of 
   photos. It stores the basic information and functionalities. It protects 
   data members to encourage clean inheritance.
*/

#ifndef PHOTO_H
#define PHOTO_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Photo
{

    protected:
        cv::Mat image;
        std::string original;
        std::vector<std::string> tags;
        std::string name;
        std::string path;

    public:
        Photo() = default;
        
        // Destructor
        virtual ~Photo() = default;
        
        // Loading and saving
        virtual bool load(const std::string& path);
        virtual bool save(const std::string& path);
        virtual void show() const;

        // Setter
        void setOriginal(const std::string& original);
        void addTag(const std::string& tag);
        void setName(const std::string& name);
        void setPath(const std::string& path);

        // Getter - Using const for robustness
        bool hasTag(const std::string& tag) const;
        const std::vector<std::string>& getTags() const;
        std::string getName() const;
        std::string getPath() const;

        // Two version
        cv::Mat& getImage();
        const cv::Mat& getImage() const;
        
        virtual std::string getType() const;
        virtual void printMetadata() const;

        // Tracking original - make Case 5 work
        virtual Photo* getSource() const{ return nullptr; }
};

#endif