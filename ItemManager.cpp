#include <iostream>
#include <iomanip>
#include <string>
#include "ItemManager.hpp"

using namespace std;

namespace ItemManager
{

    ItemNode *root = nullptr;

    void clearInput()
    {
        cin.clear();
        cin.ignore(10000, '\n');
    }

    // AVL Utility Functions

    // Compare two int and return larger int
    int getMax(int a, int b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }

    int getNodeHeight(ItemNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return node->height;
    }

    // Calculate left height minus right height to check for imbalance
    int getBalanceFactor(ItemNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        return getNodeHeight(node->left) - getNodeHeight(node->right);
    }

    // Pull the left child up and push the parent down to the right
    ItemNode *rotateRight(ItemNode *oldTop)
    {
        ItemNode *newTop = oldTop->left;
        ItemNode *orphanedChild = newTop->right;

        newTop->right = oldTop;
        oldTop->left = orphanedChild;

        oldTop->height = getMax(getNodeHeight(oldTop->left), getNodeHeight(oldTop->right)) + 1;
        newTop->height = getMax(getNodeHeight(newTop->left), getNodeHeight(newTop->right)) + 1;

        return newTop;
    }

    // Pull the right child up and push the parent down to the left
    ItemNode *rotateLeft(ItemNode *oldTop)
    {
        ItemNode *newTop = oldTop->right;
        ItemNode *orphanedChild = newTop->left;

        newTop->left = oldTop;
        oldTop->right = orphanedChild;

        oldTop->height = getMax(getNodeHeight(oldTop->left), getNodeHeight(oldTop->right)) + 1;
        newTop->height = getMax(getNodeHeight(newTop->left), getNodeHeight(newTop->right)) + 1;

        return newTop;
    }

    // Main Item Functions
    ItemNode *insertItem(ItemNode *node, int id, string name, string cat, string loc)
    {
        if (node == nullptr)
        {
            ItemNode *newNode = new ItemNode;
            newNode->itemID = id;
            newNode->itemName = name;
            newNode->category = cat;
            newNode->location = loc;
            newNode->height = 1;
            newNode->left = nullptr;
            newNode->right = nullptr;
            return newNode;
        }

        if (id < node->itemID)
        {
            node->left = insertItem(node->left, id, name, cat, loc);
        }
        else if (id > node->itemID)
        {
            node->right = insertItem(node->right, id, name, cat, loc);
        }
        else
        {
            cout << "Error: Item ID already exists.\n";
            return node;
        }

        // Update height of the current node
        node->height = 1 + getMax(getNodeHeight(node->left), getNodeHeight(node->right));

        // Check if this node is now unbalanced
        int balance = getBalanceFactor(node);

        // Left Heavy (Straight line)
        if (balance > 1 && id < node->left->itemID)
        {
            return rotateRight(node);
        }

        // Right Heavy (Straight line)
        if (balance < -1 && id > node->right->itemID)
        {
            return rotateLeft(node);
        }

        // Left-Right Zig-Zag
        if (balance > 1 && id > node->left->itemID)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right-Left Zig-Zag
        if (balance < -1 && id < node->right->itemID)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    ItemNode *searchByID(ItemNode *node, int targetID)
    {
        if (node == nullptr || node->itemID == targetID)
        {
            return node;
        }

        // Go left if the target is smaller, otherwise go right
        if (targetID < node->itemID)
        {
            return searchByID(node->left, targetID);
        }
        else
        {
            return searchByID(node->right, targetID);
        }
    }

    // Uses boolean and reference pointer to remember if we found target in any of recursion without using extra memory
    void searchByName(ItemNode *node, string targetName, bool &found)
    {
        if (node == nullptr)
        {
            return;
        }

        searchByName(node->left, targetName, found);

        if (node->itemName == targetName)
        {
            cout << "Found -> ID: " << node->itemID << " | Location: " << node->location << "\n";
            found = true;
        }

        searchByName(node->right, targetName, found);
    }

    // Helper function for delete, finds the smallest node in the right branch
    ItemNode *getMinValueNode(ItemNode *node)
    {
        ItemNode *curr = node;
        while (curr != nullptr && curr->left != nullptr)
        {
            curr = curr->left;
        }
        return curr;
    }

    ItemNode *deleteItem(ItemNode *node, int id)
    {
        if (node == nullptr)
        {
            return node;
        }

        if (id < node->itemID)
        {
            node->left = deleteItem(node->left, id);
        }
        else if (id > node->itemID)
        {
            node->right = deleteItem(node->right, id);
        }
        else
        {
            // Found the node to delete
            if (node->left == nullptr)
            {
                ItemNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr)
            {
                ItemNode *temp = node->left;
                delete node;
                return temp;
            }

            // For node with two children, copies the successor's data and delete the successor
            ItemNode *temp = getMinValueNode(node->right);
            node->itemID = temp->itemID;
            node->itemName = temp->itemName;
            node->category = temp->category;
            node->location = temp->location;
            node->right = deleteItem(node->right, temp->itemID);
        }

        if (node == nullptr)
        {
            return node;
        }

        // Rebalance the tree on the way back up
        node->height = 1 + getMax(getNodeHeight(node->left), getNodeHeight(node->right));
        int balance = getBalanceFactor(node);

        if (balance > 1 && getBalanceFactor(node->left) >= 0)
        {
            return rotateRight(node);
        }
        if (balance > 1 && getBalanceFactor(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) <= 0)
        {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void updateItem(ItemNode *root, int id)
    {
        ItemNode *target = searchByID(root, id);
        if (target != nullptr)
        {
            cout << "Enter new Location for " << target->itemName << ": ";
            getline(cin, target->location);
            cout << "Location updated successfully.\n";
        }
        else
        {
            cout << "Item not found.\n";
        }
    }

    void displayInOrder(ItemNode *node)
    {
        if (node == nullptr)
        {
            return;
        }
        displayInOrder(node->left);
        cout << left << setw(10) << node->itemID
             << setw(25) << node->itemName
             << setw(20) << node->category
             << node->location << "\n";
        displayInOrder(node->right);
    }

    void preloadData()
    {
        if (root != nullptr)
        {
            cout << "Data already loaded.\n";
            return;
        }

        cout << "Loading worst-case sequential data...\n";

        root = insertItem(root, 500, "Ergonomic Chair", "Furniture", "Zone C, Shelf 1");
        root = insertItem(root, 400, "Standing Desk", "Furniture", "Zone C, Shelf 2");
        root = insertItem(root, 300, "Mechanical Keyboard", "Electronics", "Zone A, Shelf 1");
        root = insertItem(root, 200, "Wireless Mouse", "Electronics", "Zone A, Shelf 2");
        root = insertItem(root, 100, "HDMI Cable", "Accessories", "Zone B, Shelf 1");

        cout << "Items loaded.\n";
    }

    void runMenu()
    {
        int choice = -1;
        do
        {
            cout << "\n===================================================\n";
            cout << "      [MODULE 4] WAREHOUSE ITEM DATABASE           \n";
            cout << "===================================================\n";
            cout << "  [1] Add New Item\n";
            cout << "  [2] Search by ID (O(log n))\n";
            cout << "  [3] Search by Name (O(n))\n";
            cout << "  [4] Update Item Location\n";
            cout << "  [5] Delete Item\n";
            cout << "  [6] Display All Items\n";
            cout << "  [7] Preload Test Data\n";
            cout << "  [0] Back\n";
            cout << "===================================================\n";
            cout << "  > ";
            cin >> choice;
            if (cin.fail()) 
            {
                clearInput();
                cout << "\n[!] Invalid input. Please enter a number from the menu.\n";
                choice = -1;
                continue; 
            }
            clearInput();

            if (choice == 1)
            {
                int id;
                string name, cat, loc;
                cout << "Item ID: ";
                cin >> id;
                while (cin.fail()) 
                {
                    clearInput();
                    cout << "[!] Invalid format. Item ID must be a number.\n";
                    cout << "Item ID: ";
                    cin >> id;
                }
                clearInput();

                cout << "Name: ";
                getline(cin, name);
                cout << "Category: ";
                getline(cin, cat);
                cout << "Location: ";
                getline(cin, loc);

                root = insertItem(root, id, name, cat, loc);
                cout << "Item inserted.\n";
            }
            else if (choice == 2)
            {
                int target;
                cout << "Enter Item ID: ";
                cin >> target;
                clearInput();

                ItemNode *res = searchByID(root, target);
                if (res != nullptr)
                {
                    cout << "\n--- ITEM FOUND ---\n";
                    cout << "Name: " << res->itemName << "\nLocation: " << res->location << "\n";
                }
                else
                {
                    cout << "Not found.\n";
                }
            }
            else if (choice == 3)
            {
                string target;
                cout << "Enter Exact Name: ";
                getline(cin, target);

                bool found = false;
                searchByName(root, target, found);
                if (found == false)
                {
                    cout << "No matches found.\n";
                }
            }
            else if (choice == 4)
            {
                int target;
                cout << "Enter Item ID to update: ";
                cin >> target;
                clearInput();
                updateItem(root, target);
            }
            else if (choice == 5)
            {
                int target;
                cout << "Enter Item ID to delete: ";
                cin >> target;
                clearInput();

                if (searchByID(root, target) == nullptr) 
                {
                    cout << "Error: Item ID " << target << " does not exist in the database.\n";
                } else 
                {
                    root = deleteItem(root, target);
                    cout << "Item successfully deleted.\n";
                }
            }
            else if (choice == 6)
            {
                cout << left << setw(10) << "ID" << setw(25) << "Name" << setw(20) << "Category" << "Location\n";
                cout << string(70, '-') << "\n";
                if (root == nullptr)
                {
                    cout << "Empty database.\n";
                }
                else
                {
                    displayInOrder(root);
                }
            }
            else if (choice == 7)
            {
                preloadData();
            }

        } while (choice != 0);
    }
}