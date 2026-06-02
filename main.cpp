#include <iostream>
#include "robotAssignment.hpp"
#include "ItemManager.hpp"
#include "task3.hpp"
#include "Task5_WarehouseLayout.hpp"

using namespace std;

namespace OrderManagement
{
    void runMenu();
}

int main()
{
    RobotAssignment::CircularLinkedList<RobotAssignment::Robot> robotList;
    RobotAssignment::loadRobotsFromCSV(robotList, "robots.csv");

    int choice = -1;
    do
    {
        cout << "\n===================================================\n";
        cout << "     WAREHOUSE ROBOT NAVIGATION SYSTEM             \n";
        cout << "===================================================\n";
        cout << "  [1] Order Management          (Task 1)\n";
        cout << "  [2] Robot Assignment           (Task 2)\n";
        cout << "  [3] Robot Path Navigation      (Task 3)\n";
        cout << "  [4] Item Database              (Task 4)\n";
        cout << "  [5] Warehouse Layout           (Task 5)\n";
        cout << "  [0] Exit\n";
        cout << "===================================================\n";
        cout << "  > ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
            cout << "Invalid input." << endl;
            continue;
        }
        cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
            OrderManagement::runMenu();
            break;
        case 2:
            RobotAssignment::menu(robotList);
            break;
        case 3:
            RobotNavigation::runDemo();
            break;
        case 4:
            ItemManager::runMenu();
            break;
        case 5:
            WarehouseLayout::runMenu();
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice." << endl;
        }

    } while (choice != 0);

    cout << "Goodbye!" << endl;
    return 0;
}
