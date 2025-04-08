/* Author: YiWen Lin, Syed Adnan, Poornima Kakade
   Date: 04/05/2025
   Description: 

*/

#include "Menu.h"
#include <iostream>

using namespace std;

void Menu::mainMenu(){
    int choice;
    do {
        cout << "\n----- Image Processing Menu -----" << endl;
        cout << "1. Load Image" << endl;
        cout << "2. Apply Artistic Filter" << endl;
        cout << "3. Face Detection" << endl;
        cout << "4. Inpainting" << endl;
        cout << "5. Show Image" << endl;
        cout << "6. Save Image" << endl;
        cout << "0. Exit" << endl;
        cout << "Select option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                loader.loadImage(); 
                break;
            case 2:
                filterMenu(); // move to small menu
                break;
            case 3:
                cout << "***** Under Construction. (´･Д･)」*****" << endl;
                // face.detectFaces(loader.getImage()); 
                break;
            case 4:
                cout << "***** Under Construction. (´･Д･)」*****" << endl;
                // inpainter.apply(loader.getImage()); 
                break;
            case 5:
                loader.showImage(); 
                break;
            case 6:
                loader.saveImage(); 
                break;
            case 0:
                cout << "Goodbye!" << endl; 
                break;  // return to main() and leave
            default:
                cout << "Invalid option." << endl;
        }
    } while (choice != 0);
}

void Menu::filterMenu(){
    int option;
    do {
        cout << "\n----- Artistic Filter Menu -----" << endl;
        cout << "1. Pencil Sketch" << endl;
        cout << "2. Cartoon Sketch" << endl;
        cout << "3. Oil Painting" << endl;
        cout << "4. Pop Filming" << endl;
        cout << "5. One Color Painting" << endl;
        cout << "6. Add Date" << endl;
        cout << "0. Back" << endl;
        cout << "Choose filter: ";
        cin >> option;

        switch(option){
            case 1:
                filter.pencilSketch(loader.getImage());
                break;
            case 2:
                filter.cartoonSketch(loader.getImage());
                break;
            case 3:
                filter.oilPainting(loader.getImage());
                break;
            case 4:
                filter.popFilming(loader.getImage());
                break;
            case 5:
                cout << "***** Under Construction. (´･Д･)」*****" << endl;
                /*
                char color;
                cout << "Color (r/g/b): ";
                cin >> color;
                filter.oneColorPainting(loader.getImage(), color);
                */
                break;
            case 6:
                filter.addDate(loader.getImage());
                break;
            case 0:
                break;
            default:
                cout << "Invalid option." << endl;
        }
    }while (option != 0);
}