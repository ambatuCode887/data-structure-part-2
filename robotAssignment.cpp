#include <iostream>
#include <string>
#include "robotAssignment.hpp"
#include <fstream>
#include <sstream>
using namespace std;

namespace RobotAssignment
{
    string trimCSVField(string value)
    {
        while (!value.empty() && (value.back() == '\r' || value.back() == ' ' || value.back() == '\t'))
        {
            value.pop_back();
        }

        size_t start = 0;
        while (start < value.length() && (value[start] == ' ' || value[start] == '\t'))
        {
            start++;
        }

        return value.substr(start);
    }

    void loadRobotsFromCSV(CircularLinkedList<Robot> &robotList, const string &filename)
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            string sourceFile = __FILE__;
            size_t slashPosition = sourceFile.find_last_of("\\/");
            if (slashPosition != string::npos)
            {
                string fallbackPath = sourceFile.substr(0, slashPosition + 1) + filename;
                file.open(fallbackPath);
            }
        }

        if (!file.is_open())
        {
            cout << "Warning: Could not open " << filename << ". Starting with an empty list." << endl;
            return;
        }

        string line;
        // Skip the header row (robotID,robotName,status)
        if (getline(file, line))
        {
            // Header skipped successfully
        }

        int loadCount = 0;
        while (getline(file, line))
        {
            // Skip empty lines if any
            if (line.empty())
                continue;

            stringstream ss(line);
            string id, name, status;

            // Split the row by commas
            if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, status, ','))
            {
                Robot newRobot;
                newRobot.robotID = trimCSVField(id);
                newRobot.robotName = trimCSVField(name);
                newRobot.status = trimCSVField(status);
                newRobot.numAssignedTasks = 0;
                newRobot.taskHead = nullptr;
                newRobot.taskTail = nullptr;

                // Insert into your existing circular linked list implementation
                robotList.insertAtEnd(newRobot);
                loadCount++;
            }
        }

        file.close();
        cout << "Successfully preloaded " << loadCount << " robots from " << filename << ".\n"
             << endl;
    }

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
