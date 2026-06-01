#ifndef TASK5_WAREHOUSE_LAYOUT_HPP
#define TASK5_WAREHOUSE_LAYOUT_HPP

#include <iostream>
#include <string>

// Self-implemented resizable array (replaces std::vector)
template <typename T>
class DynamicArray
{
private:
    T *arr;
    int sz;
    int cap;

    void grow()
    {
        int newCap = (cap == 0) ? 4 : cap * 2;
        T *temp = new T[newCap];
        for (int i = 0; i < sz; i++)
            temp[i] = arr[i];
        delete[] arr;
        arr = temp;
        cap = newCap;
    }

public:
    DynamicArray() : arr(nullptr), sz(0), cap(0) {}

    DynamicArray(const DynamicArray &other) : arr(nullptr), sz(0), cap(0)
    {
        if (other.cap > 0)
        {
            cap = other.cap;
            sz = other.sz;
            arr = new T[cap];
            for (int i = 0; i < sz; i++)
                arr[i] = other.arr[i];
        }
    }

    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this != &other)
        {
            delete[] arr;
            arr = nullptr;
            sz = 0;
            cap = 0;
            if (other.cap > 0)
            {
                cap = other.cap;
                sz = other.sz;
                arr = new T[cap];
                for (int i = 0; i < sz; i++)
                    arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    ~DynamicArray() { delete[] arr; }

    void push_back(const T &item)
    {
        if (sz >= cap)
            grow();
        arr[sz++] = item;
    }

    void remove_at(int index)
    {
        if (index < 0 || index >= sz)
            return;
        for (int i = index; i < sz - 1; i++)
            arr[i] = arr[i + 1];
        sz--;
    }

    T &operator[](int index) { return arr[index]; }
    const T &operator[](int index) const { return arr[index]; }
    int getSize() const { return sz; }
    bool isEmpty() const { return sz == 0; }

    int find(const T &item) const
    {
        for (int i = 0; i < sz; i++)
            if (arr[i] == item)
                return i;
        return -1;
    }

    void clear() { sz = 0; }
};

// Self-implemented linked-list queue (replaces std::queue)
// Used for BFS and level-order traversal
template <typename T>
class CustomQueue
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d) : data(d), next(nullptr) {}
    };
    Node *front;
    Node *rear;
    int count;

public:
    CustomQueue() : front(nullptr), rear(nullptr), count(0) {}

    ~CustomQueue()
    {
        while (front)
        {
            Node *temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(const T &item)
    {
        Node *n = new Node(item);
        if (!rear)
            front = rear = n;
        else
        {
            rear->next = n;
            rear = n;
        }
        count++;
    }

    T dequeue()
    {
        Node *temp = front;
        T data = temp->data;
        front = front->next;
        if (!front)
            rear = nullptr;
        delete temp;
        count--;
        return data;
    }

    bool isEmpty() const { return count == 0; }
};

// Self-implemented linked-list stack (replaces std::stack)
// Used for DFS and path reconstruction
template <typename T>
class CustomStack
{
private:
    struct Node
    {
        T data;
        Node *next;
        Node(const T &d, Node *n = nullptr) : data(d), next(n) {}
    };
    Node *topNode;
    int count;

public:
    CustomStack() : topNode(nullptr), count(0) {}

    ~CustomStack()
    {
        while (topNode)
        {
            Node *temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    void push(const T &item)
    {
        topNode = new Node(item, topNode);
        count++;
    }

    T pop()
    {
        Node *temp = topNode;
        T data = temp->data;
        topNode = topNode->next;
        delete temp;
        count--;
        return data;
    }

    bool isEmpty() const { return count == 0; }
};

// Warehouse location types
enum LocationType
{
    WAREHOUSE,
    ZONE,
    AISLE,
    SHELF
};

// Tree node representing one location in the warehouse
struct WarehouseNode
{
    int id;
    std::string name;
    LocationType type;
    WarehouseNode *parent;
    DynamicArray<WarehouseNode *> children;

    WarehouseNode(int id, const std::string &name, LocationType type,
                  WarehouseNode *parent = nullptr);
};

// Graph edge
struct Edge
{
    int destId;
    int weight;
    Edge() : destId(-1), weight(0) {}
    Edge(int d, int w) : destId(d), weight(w) {}
    bool operator==(const Edge &other) const { return destId == other.destId; }
};

// Result from pathfinding
struct PathResult
{
    DynamicArray<int> path;
    int totalCost;
    bool found;
    PathResult() : totalCost(0), found(false) {}
};

// General tree representing the warehouse hierarchy
// Warehouse -> Zones -> Aisles -> Shelves
class WarehouseTree
{
private:
    WarehouseNode *root;
    int nextId;

    WarehouseNode *findNodeById(WarehouseNode *node, int id);
    void printTree(WarehouseNode *node, std::string prefix, bool isLast);
    void preOrder(WarehouseNode *node);
    void destroyTree(WarehouseNode *node);
    void collectNodes(WarehouseNode *node, DynamicArray<WarehouseNode *> &list);

public:
    WarehouseTree();
    ~WarehouseTree();

    WarehouseNode *getRoot();
    WarehouseNode *findById(int id);
    WarehouseNode *addZone(const std::string &name);
    WarehouseNode *addAisle(int zoneId, const std::string &name);
    WarehouseNode *addShelf(int aisleId, const std::string &name);
    bool removeNode(int id);

    void displayLayout();
    void preOrderTraversal();
    void levelOrderTraversal();
    DynamicArray<WarehouseNode *> getAllNodes();
    int getNodeCount();
};

// Adjacency list graph for warehouse navigation
class WarehouseGraph
{
private:
    static const int MAX_NODES = 100;
    DynamicArray<Edge> adjList[MAX_NODES];

public:
    void addEdge(int src, int dest, int weight = 1);
    bool hasEdge(int src, int dest);
    PathResult findShortestPath(int src, int dest);
    DynamicArray<int> bfsTraversal(int start);
    DynamicArray<int> dfsTraversal(int start);
    void displayConnections(WarehouseTree &tree);
    void clear();
};

void runWarehouseMenu();

#endif