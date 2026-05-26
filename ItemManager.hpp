#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <string>

namespace ItemManager {

    struct ItemNode {
        int itemID;
        std::string itemName;
        std::string category;
        std::string location; 
        int height;
        
        ItemNode* left;
        ItemNode* right;
    };

    void runMenu();

    int getNodeHeight(ItemNode* node);
    int getBalanceFactor(ItemNode* node);
    ItemNode* rotateRight(ItemNode* y);
    ItemNode* rotateLeft(ItemNode* x);
    
    ItemNode* insertItem(ItemNode* node, int id, std::string name, std::string cat, std::string loc);
    ItemNode* searchByID(ItemNode* node, int targetID);
    void searchByName(ItemNode* node, std::string targetName, bool& found);
    void displayInOrder(ItemNode* node);
    
    ItemNode* getMinValueNode(ItemNode* node);
    ItemNode* deleteItem(ItemNode* node, int id);
    void updateItem(ItemNode* root, int id);
    void preloadData();

}

#endif