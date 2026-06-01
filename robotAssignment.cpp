#include <iostream>
#include <string>
#include "robotAssignment.hpp"
using namespace std;

namespace RobotAssignment
{
    void helperTask(Robot &robot, string taskID)
    {
        Task *newTask = new Task();

        newTask->taskID = taskID;
        newTask->next = nullptr;

        if (robot.taskHead == nullptr)
        {
            robot.taskHead = newTask;
            robot.taskTail = newTask;
        }
        else
        {
            robot.taskTail->next = newTask;
            robot.taskTail = newTask;
        }

        robot.numAssignedTasks++;
    }

    void menu(CircularLinkedList<Robot> &robotList)
    {
        int choice;

        do
        {
            cout << "\nROBOT ASSIGNMENT MODULE" << endl;
            cout << "1. Add robot" << endl;
            cout << "2. Update robot status" << endl;
            cout << "3. Assign task manually" << endl;
            cout << "4. Assign task automatically (rotating)" << endl;
            cout << "5. Display robot list" << endl;
            cout << "6. Move rotation pointer" << endl;
            cout << "7. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == 1)
            {
                Robot newRobot;

                cout << "Enter robot ID: ";
                cin >> newRobot.robotID;
                cout << "Enter robot name: ";
                cin >> newRobot.robotName;
                cout << "Enter status (available/busy/maintenance): ";
                cin >> newRobot.status;
                newRobot.numAssignedTasks = 0;

                robotList.insertAtEnd(newRobot);
            }
            else if (choice == 2)
            {
                string robotID;
                string newStatus;

                cout << "Enter robot ID: ";
                cin >> robotID;
                cout << "Enter new status (available/busy/maintenance): ";
                cin >> newStatus;

                robotList.updateStatus(robotID, newStatus);
            }
            else if (choice == 3)
            {
                string robotID;
                string taskID;

                cout << "Enter robot ID: ";
                cin >> robotID;

                cout << "Enter task ID: ";
                cin >> taskID;

                robotList.assignTask(robotID, taskID);
            }
            else if (choice == 4)
            {
                string taskID;
                cout << "Enter task ID: ";
                cin >> taskID;
                robotList.assignNextTask(taskID);
            }
            else if (choice == 5)
            {
                robotList.display();
            }
            else if (choice == 6)
            {
                robotList.moveToNextRobot();
            }
            else if (choice == 7)
            {
                cout << "Exiting Robot Assignment Module." << endl;
            }
            else
            {
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 7);
    }

} // end namespace RobotAssignment