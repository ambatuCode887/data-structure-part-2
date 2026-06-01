#include <iostream>
#include <string>

using namespace std;

struct Node {
    string command; 
    Node* next;      

    Node(string cmd) {
        command = cmd;
        next = nullptr;
    }
};

class PathStack {
private:
    Node* top;

public:
    PathStack() {
        top = nullptr;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void push(string cmd) {
        Node* newNode = new Node(cmd);
        newNode->next = top; 
        top = newNode;       
        cout << "Robot Moved: " << cmd << endl;
    }

    // POP: Remove and return the most recent movement step
    string pop() {
        if (isEmpty()) {
            cout << "The path is empty. Robot is at the start." << endl;
            return "";
        }
        
        Node* temp = top;                 // Temporarily hold the top node
        string poppedCmd = temp->command; // Get the command inside it
        top = top->next;                  // Move the top pointer to the next node
        
        delete temp;                      // Free up the memory
        return poppedCmd;                 // Return the command so we can reverse it
    }

    // PEEK: Look at the last step without removing it
    string peek() {
        if (isEmpty()) {
            return "";
        }
        return top->command;
    }
    
    // Destructor to clean up memory when the program ends
    ~PathStack() {
        while (!isEmpty()) {
            pop();
        }
    }
};

string getReverseCommand(string cmd) {
    if (cmd == "Move Forward") {
        return "Move Backward";
    } else if (cmd == "Move Backward") {
        return "Move Forward";
    } else if (cmd == "Turn Left") {
        return "Turn Right";
    } else if (cmd == "Turn Right") {
        return "Turn Left";
    } else {
        return "Stop";
    }
}

int main() {
    PathStack robotPath;

    cout << "--- Forward Navigation (To Item) ---" << endl;
    
    robotPath.push("Move Forward");
    robotPath.push("Turn Left");
    robotPath.push("Move Forward");

    cout << "\n[ALERT] Obstacle detected!" << endl;
    if (!robotPath.isEmpty()) {
        string badStep = robotPath.pop(); 
        cout << "Backtracking: Executed '" << getReverseCommand(badStep) << "'." << endl;
        
        cout << "Taking a detour..." << endl;
        robotPath.push("Turn Right");
        robotPath.push("Move Forward");
        robotPath.push("Move Forward");
    }

    cout << "\nItem successfully picked up! Preparing to return...\n" << endl;

    cout << "Last recorded step before return: " << robotPath.peek() << endl;

    cout << "\n--- Reverse Navigation (Returning to Start) ---" << endl;
    
    while (!robotPath.isEmpty()) {
        string lastStep = robotPath.pop(); 
        string reverseAction = getReverseCommand(lastStep);
        
        cout << "Popped: " << lastStep 
             << " | Executing Reverse Action: " << reverseAction << endl;
    }

    cout << "\nRobot has safely returned to the starting point." << endl;

    return 0;
}