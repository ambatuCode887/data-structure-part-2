#include <iostream>
#include <string>
#include "task3.hpp"


namespace RobotNavigation
{
    using namespace std;

    struct Node
    {
        string command;
        Node *next;

        Node(string cmd)
        {
            command = cmd;
            next = nullptr;
        }
    };

    class PathStack
    {
    private:
        Node *top;

    public:
        PathStack()
        {
            top = nullptr;
        }

        bool isEmpty()
        {
            return top == nullptr;
        }

        // Standard push for building history (Prints "Robot Moved: ...")
        void push(string cmd)
        {
            Node *newNode = new Node(cmd);
            newNode->next = top;
            top = newNode;
            cout << "Robot Moved: " << cmd << endl;
        }

        // Silent pop used during manual backtracking corrections 
        // to prevent duplicate tracking prints
        string silentPop()
        {
            if (isEmpty())
            {
                return "";
            }
            Node *temp = top;
            string poppedCmd = temp->command;
            top = top->next;
            delete temp;
            return poppedCmd;
        }

        // POP: Used for the final reverse navigation log printout
        string pop()
        {
            if (isEmpty())
            {
                cout << "The path is empty. Robot is at the start." << endl;
                return "";
            }
            return silentPop();
        }

        // PEEK: Look at the last step without removing it
        string peek()
        {
            if (isEmpty())
            {
                return "";
            }
            return top->command;
        }

        // Destructor to clean up memory when the program ends
        ~PathStack()
        {
            while (!isEmpty())
            {
                silentPop();
            }
        }
    };

    string getReverseCommand(string cmd)
    {
        if (cmd == "Move Forward")      return "Move Backward";
        if (cmd == "Move Backward")     return "Move Forward";
        if (cmd == "Turn Left")         return "Turn Right";
        if (cmd == "Turn Right")        return "Turn Left";
        return "Stop";
    }

    void runDemo()
    {
        PathStack robotPath;

        cout << "--- Forward Navigation (To Item) ---" << endl;

        robotPath.push("Move Forward");
        robotPath.push("Turn Left");
        robotPath.push("Move Forward");

        cout << "\n[ALERT] Obstacle detected!" << endl;
        if (!robotPath.isEmpty())
        {
            // Silently drop the blocked path step from stack data
            string badStep = robotPath.silentPop(); 
            cout << "Backtracking: Executed '" << getReverseCommand(badStep) << "'." << endl;

            cout << "Taking a detour..." << endl;
            robotPath.push("Turn Right");
            robotPath.push("Move Forward");
            robotPath.push("Move Forward");
        }

        cout << "\nItem successfully picked up! Preparing to return..." << endl;
        cout << "\nLast recorded step before return: " << robotPath.peek() << endl;

        cout << "\n--- Reverse Navigation (Returning to Start) ---" << endl;

        // Iterates through the corrected path history (6 steps)
        while (!robotPath.isEmpty())
        {
            string lastStep = robotPath.pop();
            string reverseAction = getReverseCommand(lastStep);

            cout << "Popped: " << lastStep
                 << " | Executing Reverse Action: " << reverseAction << endl;
        }

        cout << "\nRobot has safely returned to the starting point." << endl;
    }

} // namespace RobotNavigation