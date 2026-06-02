#ifndef ROBOTASSIGNMENT_HPP
#define ROBOTASSIGNMENT_HPP

#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

namespace RobotAssignment
{

    struct Task
    {
        string taskID;
        // pointer to the next task in the list
        Task *next;
    };

    struct Robot
    {
        string robotID;
        string robotName;
        string status;
        int numAssignedTasks = 0;
        // add task pointer to the robot structure to keep track of the tasks assigned to each robot
        Task *taskHead = nullptr; // pointer to the head of the task list for this robot
        Task *taskTail = nullptr; // pointer to the tail of the task list for this robot
    };

    template <typename T>
    class CircularLinkedList;

    void helperTask(Robot &robot, string taskID);
    void loadRobotsFromCSV(
        CircularLinkedList<Robot> &robotList,
        const string &filename);

    // circular linked list to store the robots
    template <typename T>
    struct Node // node structure for the circular linked list
    {
        T data;
        Node<T> *next;

        Node(T data)
        {
            this->data = data;
            this->next = nullptr;
        }
    };

    template <typename T>
    class CircularLinkedList
    {
    private:
        // pointer to the head of the list
        Node<T> *head;

        // pointer to the current node for rotation
        Node<T> *rotationPointer = nullptr;

        bool isDuplicateRobotID(string robotID)
        {
            if (head == nullptr)
            {
                return false;
            }

            Node<T> *current = head;

            do
            {
                if (current->data.robotID == robotID)
                {
                    return true;
                }

                current = current->next;
            } while (current != head);
            return false;
        }

        // bool isDuplicateTaskID(string taskID)
        // {
        //     if (head == nullptr)
        //     {
        //         return false;
        //     }

        //     Node<T> *current = head;
        //     do
        //     {
        //         Task *taskCurrent = current->data.taskHead;

        //         while (taskCurrent != nullptr)
        //         {
        //             if (taskCurrent->taskID == taskID)
        //             {
        //                 return true;
        //             }

        //             taskCurrent = taskCurrent->next;
        //         }

        //         current = current->next;

        //     } while (current != head);
        //     return false;
        // }

        bool isValidStatus(string status)
        {
            return status == "available" || status == "busy" || status == "maintenance";
        }

    public:
        // constructor
        CircularLinkedList()
        {
            head = nullptr;
            rotationPointer = nullptr;
        }

        // Check if a robot with the given ID exists in the list
        // Used by Task 3 to validate user input before starting navigation
        bool robotExists(string robotID)
        {
            return isDuplicateRobotID(robotID);
        }

        // void insertAtFirst(T data)
        // {
        //     if (!isValidStatus(data.status))
        //     {
        //         cout << "Invalid status. Please use available, busy, or maintenance." << endl;
        //         return;
        //     }

        //     if (isDuplicateRobotID(data.robotID))
        //     {
        //         cout << "Duplicated robot ID!" << endl;
        //         return;
        //     }

        //     Node<T> *newNode = new Node<T>(data);
        //     if (head == nullptr) // if the list is empty
        //     {
        //         head = newNode;         // then the head will point to the new node
        //         rotationPointer = head; // initialize the rotation pointer to the head
        //         newNode->next = head;   // point to the new node itself to make it circular
        //     }
        //     else
        //     {
        //         Node<T> *temp = head;      // store the current head in a temporary variable
        //         while (temp->next != head) // traverse the list until reach the last node
        //         {
        //             temp = temp->next; // move to the next node
        //         }
        //         newNode->next = head; // point the new node to the current head
        //         temp->next = newNode; // point the last node to the new node
        //         head = newNode;       // update the head to the new node
        //     }
        //     cout << "Robot added successfully." << endl;
        // }

        void insertAtEnd(T data)
        {
            if (!isValidStatus(data.status)) // check if the right status been initialise
            {
                cout << "Invalid status. Please use available, busy, or maintenance." << endl;
                return;
            }

            if (isDuplicateRobotID(data.robotID)) // check if its the same robotID
            {
                cout << "Duplicated robot ID!" << endl;
                return;
            }

            Node<T> *newNode = new Node<T>(data); // create a newNode to store the robot details
            if (head == nullptr)                  // check if empty
            {
                head = newNode;         // let newNode become the head for the first robot
                rotationPointer = head; // initialize the rotation pointer to the head
                newNode->next = head;   // point back to itself to make list circular
            }
            else
            {
                Node<T> *temp = head;      // start at first node
                while (temp->next != head) // the loop traversing to find the last node
                {
                    temp = temp->next; // then it continue until last node
                }
                temp->next = newNode; // then link the last node to the new node
                newNode->next = head; // point back to head
            }
            cout << "Robot added successfully." << endl;
        }

        void display()
        {
            if (head == nullptr)
            {
                cout << "The list is empty." << endl;
                return;
            }
            Node<T> *temp = head;
            cout << left
                 << setw(12) << "Robot ID"
                 << setw(15) << "Robot Name"
                 << setw(15) << "Status"
                 << setw(15) << "Task Count"
                 << "Task IDs" << endl;
            cout << "--------------------------------------------------------------------------" << endl;
            do
            {
                cout << left
                     << setw(12) << temp->data.robotID
                     << setw(15) << temp->data.robotName
                     << setw(15) << temp->data.status
                     << setw(15) << temp->data.numAssignedTasks;

                Task *taskTemp = temp->data.taskHead;
                if (taskTemp == nullptr)
                {
                    cout << "None";
                }
                else
                {
                    while (taskTemp != nullptr)
                    {
                        cout << taskTemp->taskID;
                        if (taskTemp->next != nullptr)
                        {
                            cout << ", ";
                        }
                        taskTemp = taskTemp->next;
                    }
                }

                cout << endl;
                temp = temp->next;
            } while (temp != head);
        }

        // let's do check status before skipping to the next node
        void updateStatus(string robotID, string newStatus)
        {
            if (head == nullptr)
            {
                cout << "The list is empty." << endl;
                return;
            }
            if (!isValidStatus(newStatus))
            {
                cout << "Invalid status. Please use available, busy, or maintenance." << endl;
                return;
            }
            Node<T> *current = head;
            do
            {
                if (current->data.robotID == robotID) // if the robot ID matches, update the status
                {
                    current->data.status = newStatus; // update the status of the robot
                    cout << "Status updated for robot " << robotID << endl;
                    return;
                }
                current = current->next;
            } while (current != head);
            cout << "Robot with ID " << robotID << " not found." << endl;
        }

        void moveToNextRobot()
        {
            if (head == nullptr)
            {
                cout << "The list is empty." << endl;
                return;
            }
            rotationPointer = rotationPointer->next; // move the rotation pointer to the next node
            cout << "Moved to the next robot: " << rotationPointer->data.robotID << " " << rotationPointer->data.robotName << endl;
        }

        void assignTask(string robotID, string taskID)
        {
            if (head == nullptr)
            {
                cout << "The list is empty." << endl;
                return;
            }

            // if (isDuplicateTaskID(taskID))
            // {
            //     cout << "Duplicated task ID. Task not assigned." << endl;
            //     return;
            // }
            Node<T> *current = head;
            do
            {
                if (current->data.robotID == robotID)
                {
                    if (current->data.status != "available")
                    {
                        cout << "Robot " << robotID << " is currently " << current->data.status << ". Task cannot be assigned." << endl;
                        return;
                    }
                    // call the helper function to assign the task to the robot
                    helperTask(current->data, taskID);
                    cout << "Task " << taskID << " assigned to robot " << robotID << endl;
                    return;
                }
                current = current->next;
            } while (current != head);
            cout << "Robot with ID " << robotID << " not found." << endl;
        }

        void assignNextTask(string taskID)
        {
            if (head == nullptr)
            {
                cout << "The list is empty." << endl;
                return;
            }

            // if (isDuplicateTaskID(taskID))
            // {
            //     cout << "Duplicated task ID. Task not assigned." << endl;
            //     return;
            // }

            Node<T> *current = rotationPointer;
            while (rotationPointer->data.status != "available")
            {
                cout << "Skipping robot " << rotationPointer->data.robotID << " as it is " << rotationPointer->data.status << "." << endl;
                rotationPointer = rotationPointer->next; // move to the next robot if the current one is not available
                if (rotationPointer == current)          // if we have looped through all robots and none are available
                {
                    cout << "No available robots to assign the task." << endl;
                    return;
                }
            }

            // assign the task to the next available robot
            helperTask(rotationPointer->data, taskID);
            cout << "Task " << taskID << " assigned to robot "
                 << rotationPointer->data.robotID
                 << ". Total assigned tasks: "
                 << rotationPointer->data.numAssignedTasks << endl;

            rotationPointer = rotationPointer->next; // move the rotation pointer to the next robot for the next assignment
        }
    };

    void menu(CircularLinkedList<Robot> &robotList);

} // end namespace RobotAssignment

#endif
