#include <iostream>
#include "snake.h"
#include "smart.h"
// #include "smart.cpp"
// #include "snake.cpp"

using namespace std;

int main()
{
    int choice = 0;
    while (choice != 3)
    {
        cout << "Welcome to Snake Game!" << endl;
        cout << "Please choose an option:" << endl;
        cout << "1. Play snake game" << endl;
        cout << "2. Watch AI play snake game" << endl;
        cout << "3. Quit" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            runSnake();

            break;
        case 2:
            runSmart();
            break;
        case 3:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
