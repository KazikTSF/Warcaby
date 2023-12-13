
#include <iostream>
#include <windows.h>

#include "Game.h"

int main()
{
    SetConsoleOutputCP(65001);
    printf("Czy widzisz te znaki? \xE2\x9B\x80 \xE2\x9B\x82 (t/n)?\n");
    char choice = 't';
    // do {
    //     std::cin >> choice;
    // } while(choice != 't' && choice != 'n');
    std::cout << "Ruchy mozna zapisywac w notacji szachowej (c3-d4), albo warcabowej(11-14)" << std::endl;
    if(choice == 't')
        Game(true);
    else
        Game(false);
    return 0;
}
