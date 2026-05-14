#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <string>

namespace ItemManager {

    struct ItemNode {
        int itemID;
        std::string itemName;
        std::string category;
        std::string location;
        
        ItemNode* left;
        ItemNode* right;
    };

    void runMenu();

    ItemNode* insertItem(ItemNode* root, int id, std::string name, std::string cat, std::string loc);
    ItemNode* searchByID(ItemNode* root, int targetID);
    void searchByName(ItemNode* root, std::string targetName, bool& found);
    void displayInOrder(ItemNode* root);
    ItemNode* deleteItem(ItemNode* root, int targetID);
    ItemNode* findMin(ItemNode* root);

}

#endif