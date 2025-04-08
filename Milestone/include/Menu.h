/* Author: YiWen Lin
   Date: 04/05/2025
   Description: This file defines a class named Menu.
*/

#ifndef MENU_H
#define MENU_H
#include "ImageLoader.h"
#include "ArtisticFilter.h"

class Menu{
public:
    void mainMenu();
private:
    void filterMenu();

    ImageLoader loader;
    ArtisticFilter filter;
    // FaceProcessor face;
    // Inpainter inpainter;
};

#endif