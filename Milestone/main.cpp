/* Author: YiWen Lin, Syed Adnan, Poornima Kakade
   Date: 04/05/2025
   Description: When users run the program, the main function will creat 
   an object of menu to interact with the menu. When user chooce "Exit" 
   in "Image Processing Menu", process will return to the main function to 
   end the program.
*/

#include <Menu.h>
#include <iostream>

using namespace std;

int main(){
    Menu menu;
    menu.mainMenu();
    
    return 0;
}