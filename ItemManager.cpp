#include <iostream>
#include <iomanip>
#include <string>
#include "ItemManager.hpp"

using namespace std;

namespace ItemManager {

    // Global root pointer for the Item Database
    ItemNode* root = nullptr;

    // Helper function to clear console input
    void clearInput() {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    // 1. BST INSERTION O(log n)
    ItemNode* insertItem(ItemNode* node, int id, string name, string cat, string loc) {
        if (node == nullptr) {
            ItemNode* newNode = new ItemNode;
            newNode->itemID = id;
            newNode->itemName = name;
            newNode->category = cat;
            newNode->location = loc;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }

        if (id < node->itemID) {
            node->left = insertItem(node->left, id, name, cat, loc);
        } else if (id > node->itemID) {
            node->right = insertItem(node->right, id, name, cat, loc);
        } else {
            cout << "  [Error] Item ID " << id << " already exists in the system.\n";
        }
        return node;
    }

    // 2. BST SEARCH BY ID O(log n)
    ItemNode* searchByID(ItemNode* node, int targetID) {
        if (node == nullptr || node->itemID == targetID) {
            return node;
        }
        if (targetID < node->itemID) {
            return searchByID(node->left, targetID);
        }
        return searchByID(node->right, targetID);
    }

    // 3. TREE TRAVERSAL SEARCH BY NAME O(n)
    // Note: Since the tree is sorted by ID, searching by string requires checking every node.
    void searchByName(ItemNode* node, string targetName, bool& found) {
        if (node == nullptr) return;

        searchByName(node->left, targetName, found);

        if (node->itemName == targetName) {
            cout << "  Found: ID [" << node->itemID << "] | Location: " << node->location << "\n";
            found = true;
        }

        searchByName(node->right, targetName, found);
    }

    // 4. IN-ORDER TRAVERSAL (Displays items sorted by ID)
    void displayInOrder(ItemNode* node) {
        if (node == nullptr) return;

        displayInOrder(node->left);
        
        cout << left << setw(10) << node->itemID 
             << setw(25) << node->itemName 
             << setw(20) << node->category 
             << node->location << "\n";
             
        displayInOrder(node->right);
    }

    void runMenu() {
        int choice;
        do {
            cout << "\n===================================================\n";
            cout << "      [MODULE 4] ITEM SEARCH & MANAGEMENT          \n";
            cout << "===================================================\n";
            cout << "  [1] Add New Item\n";
            cout << "  [2] Search Item by ID - O(log n)\n";
            cout << "  [3] Search Item by Name - O(n)\n";
            cout << "  [4] Display All Items (Sorted by ID)\n";
            cout << "  [0] Return to Main Menu\n";
            cout << "===================================================\n";
            cout << "  > ";
            cin >> choice;
            clearInput();

            if (choice == 1) {
                int id;
                string name, cat, loc;
                cout << "\n  Enter Item ID (Number): ";
                cin >> id;
                clearInput();
                cout << "  Enter Item Name: ";
                getline(cin, name);
                cout << "  Enter Category: ";
                getline(cin, cat);
                cout << "  Enter Location (e.g., Zone A, Zone B): ";
                getline(cin, loc);

                root = insertItem(root, id, name, cat, loc);
                cout << "  [Success] Item added to the database.\n";

            } else if (choice == 2) {
                int target;
                cout << "\n  Enter Item ID to search: ";
                cin >> target;
                clearInput();

                ItemNode* result = searchByID(root, target);
                if (result != nullptr) {
                    cout << "\n  [ITEM FOUND]\n";
                    cout << "  Name:     " << result->itemName << "\n";
                    cout << "  Category: " << result->category << "\n";
                    cout << "  Location: " << result->location << "\n";
                } else {
                    cout << "\n  [ERROR] Item ID " << target << " not found in the warehouse.\n";
                }

            } else if (choice == 3) {
                string target;
                cout << "\n  Enter Exact Item Name to search: ";
                getline(cin, target);
                
                bool found = false;
                cout << "\n  [SEARCH RESULTS]\n";
                searchByName(root, target, found);
                if (!found) {
                    cout << "  No items matched the name '" << target << "'.\n";
                }

            } else if (choice == 4) {
                cout << "\n========================================================================\n";
                cout << left << setw(10) << "Item ID" << setw(25) << "Item Name" << setw(20) << "Category" << "Location\n";
                cout << "========================================================================\n";
                if (root == nullptr) {
                    cout << "  Database is currently empty.\n";
                } else {
                    displayInOrder(root);
                }
                cout << "========================================================================\n";
            }

        } while (choice != 0);
    }
}