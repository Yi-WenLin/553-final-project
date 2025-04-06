/* Author: YiWen Lin, Syed Adnan, Poornima Kakade
   Date: 04/05/2025
   Description: 

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