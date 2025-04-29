#include "Photo.h"


#include "FilteredPhoto.h"
#include "TransferredPhoto.h"

#include "ImageCollager.h" 

#include "ArtisticFilter.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
 // Include the ImageCollager header

using namespace std;

void filterMenu(string& currentName);
void mainMenu(string& currentName);

// SourceName() sourceName
int main()
{
    // map to record all photos name with its ptr - | {{name, Photo object ptr}} | {...} |
    map<string, Photo*> photoMap;

    // Map to track original to derivative photo - | {SourceName, [Names of Derivatives]} | {...} |
    map<string, vector<string>> originToDerivatives;

    // Current selected original photo name
    string currentPhotoName;

    ArtisticFilter filter;

    bool running = true;  // Sentinel to control Main Menu repetition

    while (running)
    {
        mainMenu(currentPhotoName);
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        // Load a Photo
        case 1:
        {
            string name, path, tag;
            std::cout << "Please name the photo: ";
            getline(cin, name);
            std::cout << "Please enter the file path: ";
            getline(cin, path);
            std::cout << "Please enter a tag for this photo: ";
            getline(cin, tag);

            Photo* photo = new Photo();
            if (photo->load(path))  // bool
            {
                photo->setName(name);
                photo->setOriginal("Original");
                photo->addTag(tag);

                photoMap[name] = photo;

                currentPhotoName = name;
                std::cout << "Photo loaded as: " << name << std::endl;
            }
            else
            {
                delete photo;
            }
            break;
        }

        // List all photos
        case 2:
        {
            std::cout << "All Original Photo:\n";
            for (const auto& [name, photo] : photoMap)
            {
                if (photo->getType() == "Photo")
                {
                    std::cout << "- " << name << std::endl;
                }
            }

            std::cout << "Photos Structure:\n";
            for (const auto& [origin, derivatives] : originToDerivatives)
            {
                std::cout << "- " << origin << " [Original]" << std::endl;
                for (const auto& d : derivatives)
                {
                    std::cout << "    - " << d << " [" << photoMap[d]->getType() << "]" << std::endl;
                }
            }
            break;
        }

        // Select Current Photo
        case 3:
        {
            int search;
            std::cout << "Do you want to search by: \n 1. Name \n 2. Tag \nOption: ";
            // Search by Name
            std::cin >> search;
            std::cin.ignore();

            if (search == 1)  // Name
            {
                // Show all photo names without structure
                for (const auto& [name, photo] : photoMap)
                {
                    std::cout << " - " << name << std::endl;
                }

                string name;
                std::cout << "Please enter photo name to select: ";
                getline(cin, name);
                if (photoMap.count(name))
                {
                    currentPhotoName = name;
                    std::cout << "Photo '" << name << "' selected.\n";
                }
                else
                {
                    std::cout << "Photo not found." << std::endl;
                }
            }
            else if (search == 2)  // Tag
            {
                string tag;
                std::cout << "Please enter a tag to search: ";
                getline(cin, tag);
                std::cout << "Photos with tag '" << tag << "': " << std::endl;
                for (auto& [name, photo] : photoMap)
                {
                    if (photo->hasTag(tag))
                    {
                        std::cout << " - " << name << std::endl;
                    }
                }
                string name;
                std::cout << "Please enter photo name to select: ";
                getline(cin, name);
                if (photoMap.count(name))
                {
                    currentPhotoName = name;
                    std::cout << "Photo '" << name << "' selected.\n";
                }
                else
                {
                    std::cout << "Photo not found." << std::endl;
                }
            }
            else
            {
                std::cout << "Invalid option. " << std::endl;
            }
            break;
        }

        // Show Current Photo
        case 4:
        {
            if (photoMap.count(currentPhotoName))
            {
                photoMap[currentPhotoName]->show(); // virtual can identify the object
            }
            else
            {
                std::cout << "No photo selected." << std::endl;
            }
            break;
        }

        // Apply Artistic Filter
        case 5:
        {
            if (!photoMap.count(currentPhotoName))
            {
                std::cout << "No photo selected." << std::endl;
                break;
            }

            string newName;
            std::cout << "Please name for the new filtered photo: ";
            getline(cin, newName);

            // Show Filter Menu
            filterMenu(currentPhotoName);
            int type;
            cin >> type;
            cin.ignore();

            Photo* base = photoMap[currentPhotoName];
            cv::Mat copy = base->getImage().clone();
            string filterName;

            if (type == 1)
            {
                filter.cartoonSketch(copy);
                filterName = "Cartoon Sketch";
            }
            else if (type == 2)
            {
                filter.oilPainting(copy);
                filterName = "Oil Painting";
            }
            else if (type == 3)
            {
                filter.popFilming(copy);
                filterName = "Pop Filming";
            }
            else if (type == 4)
            {
                filter.addDate(copy);
                filterName = "Date Adding";
            }
            else {
                std::cout << "Invalid filter. " << std::endl;
                break;
            }

            FilteredPhoto* filtered = new FilteredPhoto(*base, filterName);
            filtered->getImage() = copy;
            filtered->setSource(*base);
            filtered->addTag(filterName);

            // Add to PhotoMap
            photoMap[newName] = filtered;

            // Add to dictionary
            Photo* source = base->getSource();
            if (source)  // base is a filtered or transferred image
            {

                originToDerivatives[source->getName()].push_back(newName);
            }
            else  // base is an original image
            {

                originToDerivatives[base->getName()].push_back(newName);
            }

            std::cout << "New filtered photo created as '" << newName << "'" << std::endl;
            break;
        }

        // Create Collage (Original vs Filtered)
        case 11:
        {
            if (!photoMap.count(currentPhotoName)) {
                std::cout << "No photo selected.\n";
                break;
            }

            // Create ImageCollager instance
            ImageCollager collager;

            // Load the current photo (original photo)
            collager.loadImage(photoMap[currentPhotoName]->getImage());

            // Get the name of the filtered or transferred photo to create a collage with
            string secondImageName;
            std::cout << "Enter the name of the filtered or transferred photo: ";
            getline(std::cin, secondImageName);

            // Check if the second image exists
            if (!photoMap.count(secondImageName)) {
                std::cout << "The selected filtered/modified photo does not exist.\n";
                break;
            }

            // Load the second image (filtered or transferred photo)
            collager.loadSecondImage(photoMap[secondImageName]->getImage());

            // Create the collage and display it
            collager.createCollage();

            std::cout << "Collage created successfully!\n";
            break;
        }

        // View MetaData
        case 7:
        {
            if (!photoMap.count(currentPhotoName))
            {
                std::cout << "No photo selected.\n";
                break;
            }
            photoMap[currentPhotoName]->printMetadata();
            break;
        }

        // Download Current Photo
        case 8:
        {
            if (!photoMap.count(currentPhotoName))
            {
                std::cout << "No photo selected. " << std::endl;
                break;
            }
            string path;
            std::cout << "Enter path to save: ";
            getline(cin, path);
            photoMap[currentPhotoName]->save(path);
            break;
        }

        // Delete a Photo
        case 9:
        {
            string name;
            std::cout << "Enter name of photo to delete: ";
            getline(cin, name);
            if (photoMap.count(name))
            {
                delete photoMap[name];
                photoMap.erase(name);

                if (name == currentPhotoName)
                    currentPhotoName.clear();

                for (auto& [origin, vec] : originToDerivatives)
                {
                    vec.erase(remove(vec.begin(), vec.end(), name), vec.end());
                }
                std::cout << "Deleted photo '" << name << std::endl;
            }
            else
            {
                std::cout << "Photo not found.\n";
            }
            break;
        }

        // Add Tag to Current Photo
        case 10:
        {
            if (!photoMap.count(currentPhotoName))
            {
                std::cout << "No photo selected. " << std::endl;
                break;
            }
            string tag;
            std::cout << "Enter tag to add: ";
            getline(cin, tag);
            photoMap[currentPhotoName]->addTag(tag);
            std::cout << "Tag added.\n";
