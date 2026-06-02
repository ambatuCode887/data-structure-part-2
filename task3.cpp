#include "task3.hpp"
#include <iostream>
#include <string>
#include <fstream>

namespace RobotNavigation {
using namespace std;

// Helper function to check if a destination exists in Task 5's warehouse_layout.csv
bool isValidDestination(const string &dest) {
  ifstream file("warehouse_layout.csv");
  if (!file.is_open()) return true; // Fail open if file is missing so we don't break the whole program

  string line;
  getline(file, line); // skip header
  while (getline(file, line)) {
    // CSV format: id,name,type,parentId -> we want to match 'name' (second column)
    size_t firstComma = line.find(',');
    if (firstComma != string::npos) {
      size_t secondComma = line.find(',', firstComma + 1);
      if (secondComma != string::npos) {
        string name = line.substr(firstComma + 1, secondComma - firstComma - 1);
        if (name == dest) return true;
      }
    }
  }
  return false;
}

struct Node {
  string command;
  Node *next;

  Node(string cmd) {
    command = cmd;
    next = nullptr;
  }
};


class PathStack {
private:
  Node *top;

public:
  PathStack() { top = nullptr; }

  // Returns true if no steps have been recorded yet
  bool isEmpty() { return top == nullptr; }

  // PUSH: Record a new movement step and print it to the log
  void push(const string &cmd) {
    Node *newNode = new Node(cmd);
    newNode->next = top;
    top = newNode;
    cout << "  [MOVE] " << cmd << endl;
  }

  // SILENT POP: Remove and return the top step without printing.
  // Used during obstacle backtracking so we don't double-log.
  string silentPop() {
    if (isEmpty())
      return "";

    Node *temp = top;
    string poppedCmd = temp->command;
    top = top->next;
    delete temp;
    return poppedCmd;
  }

  // POP: Remove and return the top step (used during reverse navigation)
  string pop() {
    if (isEmpty()) {
      cout << "  Stack is empty — robot is back at the start." << endl;
      return "";
    }
    return silentPop();
  }

  // PEEK: Look at the most recent step without removing it
  string peek() {
    if (isEmpty())
      return "(empty)";
    return top->command;
  }

  // Destructor: free all remaining nodes when PathStack goes out of scope
  ~PathStack() {
    while (!isEmpty())
      silentPop();
  }
};

string getReverseCommand(const string &cmd) {
  if (cmd == "Move Forward")
    return "Move Backward";
  if (cmd == "Move Backward")
    return "Move Forward";
  if (cmd == "Turn Left")
    return "Turn Right";
  if (cmd == "Turn Right")
    return "Turn Left";
  return "Stop";
}

void runDemo(const string &robotID, const string &destination) {
  PathStack robotPath;

  // --- Header: show which robot and where it is going ---
  cout << "\n=========================================" << endl;
  cout << "   ROBOT NAVIGATION & PATH TRACKING     " << endl;
  cout << "=========================================" << endl;
  cout << "  Robot ID   : " << robotID << endl;
  cout << "  Destination: " << destination << endl;
  cout << "=========================================\n" << endl;

  // ---- PHASE 1: Forward Navigation ----
  cout << "--- Phase 1: Forward Navigation (Moving to Item) ---" << endl;
  cout << "Commands:" << endl;
  cout << "  [1] Move Forward    [2] Turn Left" << endl;
  cout << "  [3] Turn Right      [4] Move Backward" << endl;
  cout << "  [5] Obstacle!       [6] View Last Step" << endl;
  cout << "  [7] Item Reached    (ends forward navigation)\n" << endl;

  int navChoice;
  bool itemReached = false;

  do {
    cout << "[" << robotID << " -> " << destination << "] > ";
    if (!(cin >> navChoice)) {
      cin.clear();
      cin.ignore(10000, '\n');
      cout << "  Invalid input. Please enter a number.\n";
      continue;
    }
    cin.ignore(10000, '\n');

    switch (navChoice) {
    case 1:
      robotPath.push("Move Forward");
      break;

    case 2:
      robotPath.push("Turn Left");
      break;

    case 3:
      robotPath.push("Turn Right");
      break;

    case 4:
      robotPath.push("Move Backward");
      break;

    case 5:
      // Obstacle handling: undo the last step and backtrack
      cout << "\n  [ALERT] Obstacle detected!" << endl;
      if (!robotPath.isEmpty()) {
        string blockedStep = robotPath.silentPop();
        cout << "  Backtracking: reversing '" << blockedStep
             << "' -> executing '" << getReverseCommand(blockedStep) << "'"
             << endl;
        cout << "  Path corrected. Choose a detour.\n" << endl;
      } else {
        cout << "  No steps to backtrack - robot is at the starting point.\n"
             << endl;
      }
      break;

    case 6:
      // Peek: show the most recent step without altering the stack
      cout << "  Last recorded step: " << robotPath.peek() << endl;
      break;

    case 7:
      itemReached = true;
      break;

    default:
      cout << "  Invalid command. Please choose 1–7." << endl;
    }

  } while (!itemReached);

  // ---- Arrival confirmation ----
  cout << "\n  [SUCCESS] Robot " << robotID << " has arrived at '"
       << destination << "' and picked up the item." << endl;

  // ---- PHASE 2: Reverse Navigation ----
  cout << "\n--- Phase 2: Reverse Navigation (Returning to Start) ---" << endl;

  if (robotPath.isEmpty()) {
    cout << "  No path was recorded. Robot remains at current position."
         << endl;
  } else {
    int step = 1;
    while (!robotPath.isEmpty()) {
      string lastStep = robotPath.pop(); // retrieve last forward step
      string reverseAction = getReverseCommand(lastStep);

      cout << "  Step " << step++ << ": reverse of '" << lastStep
           << "' -> executing '" << reverseAction << "'" << endl;
    }

    cout << "\n  [DONE] Robot " << robotID
         << " has safely returned to the starting point." << endl;
  }

  cout << "=========================================\n" << endl;
}

void runMenu(
    RobotAssignment::CircularLinkedList<RobotAssignment::Robot> &robotList) {
  // Show the robot list from Task 2
  cout << "\n--- Available Robots (from Task 2: Robot Assignment) ---" << endl;
  robotList.display();

  string navRobotID, navDestination;

  // Keep prompting until the user enters a Robot ID that actually exists
  do {
    cout << "\nEnter the Robot ID to navigate: ";
    cin >> navRobotID;
    cin.ignore(10000, '\n');

    if (!robotList.robotExists(navRobotID))
      cout << "  [!] Robot ID '" << navRobotID
           << "' not found. Please enter a valid Robot ID from the list above.\n";

  } while (!robotList.robotExists(navRobotID));

  do {
    cout << "Enter item destination (e.g. Zone A - Receiving, Shelf A1-01): ";
    getline(cin, navDestination);
    
    if (!isValidDestination(navDestination)) {
      cout << "  [!] Destination '" << navDestination 
           << "' not found in warehouse_layout.csv. Please try again.\n\n";
    }
  } while (!isValidDestination(navDestination));

  runDemo(navRobotID, navDestination);
}

} // namespace RobotNavigation