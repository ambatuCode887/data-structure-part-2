#include <iostream>
#include <string>

// Include everyone's connector files
//#include "OrderManager.hpp"
//#include "RobotManager.hpp"
//#include "Navigation.hpp"
#include "ItemManager.hpp"
//#include "WarehouseLayout.hpp"

using namespace std;

void displayMenu() {
    cout << "\n===================================================\n";
    cout << "       WAREHOUSE ROBOT NAVIGATION SYSTEM           \n";
    cout << "===================================================\n";
    cout << "  [1] Order Management (Queue)\n";
    cout << "  [2] Robot Assignment (Circular Queue)\n";
    cout << "  [3] Robot Navigation & Reversal (Stack)\n";
    cout << "  [4] Item Search & Management (Binary Search Tree)\n";
    cout << "  [5] Warehouse Layout (Graph/Tree)\n";
    cout << "  [0] Exit System\n";
    cout << "===================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    int choice;

    do {
        displayMenu();
        cin >> choice;

        // Clear input buffer to prevent infinite loops on bad input
        cin.clear(); 
        cin.ignore(10000, '\n');

        switch (choice) {
            case 1:
                cout << "\n--- Launching Order Management ---\n";
                // Call OrderManager::runMenu();
                break;
            case 2:
                cout << "\n--- Launching Robot Assignment ---\n";
                // Call RobotManager::runMenu();
                break;
            case 3:
                cout << "\n--- Launching Navigation ---\n";
                // Call Navigation::runMenu();
                break;
            case 4:
                cout << "\n--- Launching Item Search ---\n";
                ItemManager::runMenu(); 
                break;
            case 5:
                cout << "\n--- Launching Warehouse Layout ---\n";
                // Call WarehouseLayout::runMenu();
                break;
            case 0:
                cout << "\nShutting down warehouse systems. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid choice. Please enter 0-5.\n";
        }
    } while (choice != 0);

    return 0;
}