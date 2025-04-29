/* Author: YiWen Lin
   Date: 04/27/2025
   Description: This file is the main control of the system.
*/

#include "Photo.h"
#include "FilteredPhoto.h"
#include "TransferredPhoto.h"
#include "ArtisticFilter.h"
#include <iostream>
#include <map>
#include <unordered_map>  // for naming
#include <unordered_set>  // for naming
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Function Declaration
void filterMenu(string& currentName);
void mainMenu(string& currentName);
std::string uniqueNaming(const std::string& checkName, unordered_map<string, int>& baseCounter, unordered_set<string>& usedNames);

int main()
{
    // map to record all photos name with its ptr - | {{name, Photo object ptr}} | {...} |
    map<string, Photo*> photoMap;

    // Map to track original to derivative photo - | {SourceName, [Names of Derivatives]} | {...} |
    map<string, vector<string>> originToDerivatives;

    // A map that store the count of base name for easy adding - | {base name, count} | {...} |
    unordered_map<string, int> baseCounter;
    // A set that store all used name - | {name1} | {name2} |
    unordered_set<string> usedNames;
    
    //Current selected original photo name
    string currentPhotoName;

    ArtisticFilter filter;
    
    bool running = true;  // Sentinel to control Main Menu repetition

    while(running)
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
                if(photo->load(path))  // bool
                {
                    // Name checking
                    name = uniqueNaming(name, baseCounter, usedNames);
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
                for(const auto& [name, photo] : photoMap)
                {
                    if(photo->getType() == "Photo")
                    {
                        std::cout << "- " << name << std::endl;
                    }
                }

                std::cout << "Photos Structure:\n";
                for(const auto& [origin, derivatives] : originToDerivatives)
                {
                    std::cout << "- " << origin << " [Original]" << std::endl;
                    for(const auto& d : derivatives)
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

                if(search == 1)  // Name
                {
                    // Show all photo names without structure
                    for(const auto& [name, photo] : photoMap)
                    {
                        std::cout << " - " << name << std::endl;
                    }

                    string name;
                    std::cout << "Please enter photo name to select: ";
                    getline(cin, name);
                    if(photoMap.count(name))
                    {
                        currentPhotoName = name;
                        std::cout << "Photo '" << name << "' selected.\n";
                    }
                    else
                    {
                        std::cout << "Photo not found." << std::endl;
                    }
                }
                else if(search == 2)  // Tag
                {
                    string tag;
                    std::cout << "Please enter a tag to search: ";
                    getline(cin, tag);
                    std::cout << "Photos with tag '" << tag << "': " << std::endl;
                    for(auto& [name, photo] : photoMap)
                    {
                        if(photo->hasTag(tag))
                        {
                            std::cout << " - " << name << std::endl;
                        }
                    }
                    string name;
                    std::cout << "Please enter photo name to select: ";
                    getline(cin, name);
                    if(photoMap.count(name))
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
                    break;
                }
                break;
            }

            // Show Current Photo
            case 4:{
                if(photoMap.count(currentPhotoName))
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
            case 5:{
                if(!photoMap.count(currentPhotoName))
                {
                    std::cout << "No photo selected." << std::endl;
                    break;
                }

                string newName;
                std::cout << "Please name for the new filtered photo: ";
                getline(cin, newName);
                // Name checking
                newName = uniqueNaming(newName, baseCounter, usedNames);

                // Show Filter Menu
                filterMenu(currentPhotoName);
                int type;
                cin >> type;
                cin.ignore();

                Photo* base = photoMap[currentPhotoName];
                cv::Mat copy = base->getImage().clone();
                string filterName;

                if(type == 1)
                {
                    filter.cartoonSketch(copy);
                    filterName = "Cartoon Sketch";
                }
                else if(type == 2)
                {
                    filter.oilPainting(copy);
                    filterName = "Oil Painting";
                }
                else if(type == 3)
                {
                    filter.popFilming(copy);
                    filterName = "Pop Filming";
                }
                else if(type == 4)
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
                if(source)  // base is a filtered or transferred image
                {
                    
                    originToDerivatives[source->getName()].push_back(newName);
                }
                else  // base is an original image
                {
                    
                    originToDerivatives[base->getName()].push_back(newName);
                }
                //originToDerivatives[base->getName()].push_back(newName);  OR (base->getSource())->getName();
                
                std::cout << "New filtered photo created as '" << newName << "'" << std::endl;
                break;
            }
            // Apply Style Transfer
            case 6:{
                if (!photoMap.count(currentPhotoName)) {
                    std::cout << "No photo selected.\n";
                    break;
                }
                
                string newName;
                std::cout << "Please name for the new tranferred photo: ";
                getline(cin, newName);
                // Name checking
                newName = uniqueNaming(newName, baseCounter, usedNames);
                

                int type;
                std::cout << "Please choose a Transfer Style:\n 1. Mosaic Art\n 2. Modern Art \nOption: ";
                cin >> type;
                cin.ignore();
                string model_path, model_name;

                if(type == 1)
                {
                    model_name = "Mosaic Style";
                    model_path = "/Users/carollyn/Desktop/cppproject/onnxtest2/models/mosaic.onnx";
                }
                else if(type == 2)
                {
                    model_name = "Modern Art Style";
                    model_path = "/Users/carollyn/Desktop/cppproject/onnxtest2/models/candy.onnx";
                }
                else
                {
                    std::cout << "Invalid style. " << std::endl;
                    break;
                }

                Photo* base = photoMap[currentPhotoName];
                TransferredPhoto* transferred = new TransferredPhoto(model_path);

                // Apply style transfer
                cv::Mat styled = transferred->apply(base->getImage());
                transferred->getImage() = styled;
                transferred->setSource(*base);
                transferred->addTag("Transfer Style");
                transferred->setModelName(model_name);

                // Add into photo map
                photoMap[newName] = transferred;

                // Add to dictionary
                Photo* source = base->getSource();
                if(source)  // base is a filtered or transferred image
                {
                    
                    originToDerivatives[source->getName()].push_back(newName);
                }
                else  // base is an original image
                {
                    
                    originToDerivatives[base->getName()].push_back(newName);
                }

                std::cout << "New transferred photo created as '" << newName << "'" << std::endl;
                break;
            }

            // View MetaData
            case 7:{
                if(!photoMap.count(currentPhotoName))
                {
                    std::cout << "No photo selected.\n";
                    break;
                }
                photoMap[currentPhotoName]->printMetadata();
                break;
            }

            // Download Current Photo
            case 8:{
                if(!photoMap.count(currentPhotoName))
                {
                    std::cout << "No photo selected. " << std::endl;
                    break;
                }
                string path;
                std::cout << "Please enter path to save (with suffix): ";
                getline(cin, path);
                photoMap[currentPhotoName]->save(path);
                break;
            }

            // Delete a Photo
            case 9:{
                string name;
                std::cout << "Enter the name of photo to delete: ";
                getline(cin, name);

                if(photoMap.count(name))
                {
                    delete photoMap[name];
                    photoMap.erase(name);

                    if(name == currentPhotoName)
                        currentPhotoName.clear();
                    
                    for(auto& [origin, vec] : originToDerivatives)
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
                if(!photoMap.count(currentPhotoName))
                {
                    std::cout << "No photo selected. " << std::endl;
                    break;
                }

                string tag;
                std::cout << "Enter tag to add: ";
                getline(cin, tag);

                photoMap[currentPhotoName]->addTag(tag);
                std::cout << "Tag added.\n";
                break;
            }

            // Exit
            case 0:
            {
                running = false;
                break;
            }

            default:
            {
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
    }

    // Destructor
    for(auto& [name, ptr] : photoMap)
    {
        delete ptr;
    }

    return 0;
}


void filterMenu(std::string& currentName)
{
    const std::string bold = "\033[1m";
    const std::string italic = "\033[3m";
    const std::string reset = "\033[0m";
    const std::string optionColor = "\033[36m"; // cyan

    std::cout << "\n--------------------------------------------\n";
    std::cout << "|            " << bold << "Filter Menu" << reset << "                   |\n";
    //std::cout << "| Current: " << (currentName.empty() ? "None" : currentName);
    //std::cout << std::string(33 - (currentName.empty() ? 4 : currentName.length()), ' ') << "|\n";
    std::cout << "|                                          |\n";
    std::cout << "|------------------------------------------|\n";
    std::cout << "| " << optionColor << "1. Cartoon Sketch" << reset << "                        |\n";
    std::cout << "| " << optionColor << "2. Oil Painting" << reset << "                          |\n";
    std::cout << "| " << optionColor << "3. Pop Filming" << reset << "                           |\n";
    std::cout << "| " << optionColor << "4. Add Date" << reset << "                              |\n";
    //std::cout << "|                              " << italic << "0. Back" << reset << "     |\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Choose a filter option: ";
}

void mainMenu(std::string& currentName)
{
    const std::string bold = "\033[1m";
    const std::string italic = "\033[3m";
    const std::string reset = "\033[0m";
    const std::string optionColor = "\033[32m"; // green

    std::cout << "\n--------------------------------------------------------------\n";
    std::cout << "|                  " << bold << "Image Processing System" << reset << "                   |\n";
    std::cout << "| Current: " << (currentName.empty() ? "None" : currentName);
    std::cout << std::string(50 - (currentName.empty() ? 4 : currentName.length()), ' ') << "|\n";
    std::cout << "|------------------------------------------------------------|\n";
    std::cout << "| " << optionColor << "1. Load Photo" << reset << "                                              |\n";
    std::cout << "| " << optionColor << "2. List Loaded Photos" << reset << "                                      |\n";
    std::cout << "| " << optionColor << "3. Select Current Photo" << reset << "                                    |\n";
    std::cout << "| " << optionColor << "4. Show Current Photo" << reset << "                                      |\n";
    std::cout << "| " << optionColor << "5. Apply Artistic Filter" << reset << "                                   |\n";
    std::cout << "| " << optionColor << "6. Apply Style Transfer" << reset << "                                    |\n";
    std::cout << "| " << optionColor << "7. View Metadata" << reset << "                                           |\n";
    std::cout << "| " << optionColor << "8. Save Current Photo" << reset << "                                      |\n";
    std::cout << "| " << optionColor << "9. Delete a Photo" << reset << "                                          |\n";
    std::cout << "| " << optionColor << "10. Add Tag" << reset << "                                                |\n";
    std::cout << "|                                                " << italic << "0. Exit" << reset << "     |\n";
    std::cout << "--------------------------------------------------------------\n";
    std::cout << "Choose an option: ";
}

std::string uniqueNaming(const std::string& checkName, unordered_map<string, int>& baseCounter, unordered_set<string>& usedNames)
{
    std::string FinalName = checkName;

    if (usedNames.count(checkName))
    {
        // Name already exists
        int idx = baseCounter[checkName] + 1; // start from next index

        while(true)
        {
            FinalName = checkName + "" + to_string(idx);
                
            // Double check in usedNames
            if(!usedNames.count(FinalName))
            {
                std::cout << "Name is already existed. We will rename it as '" << FinalName << "' " << std::endl;
                break;
            }
            idx++; // add until unique
        }

        baseCounter[checkName] = idx; // Add to baseName with count idx
    }
    // Name does not already exists
    else
    {
        baseCounter[checkName] = 0; // Add to baseName with count 0
    }

    usedNames.insert(FinalName);  // Add to usedNames
    return FinalName;
}