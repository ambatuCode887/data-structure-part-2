#include "Task5_WarehouseLayout.hpp"

using namespace std;

// ============================================================
// WarehouseNode
// ============================================================

WarehouseNode::WarehouseNode(int id, const string &name, LocationType type, WarehouseNode *parent)
    : id(id), name(name), type(type), parent(parent) {}

// ============================================================
// WarehouseTree
// ============================================================

WarehouseTree::WarehouseTree() : nextId(0), root(nullptr)
{
    root = new WarehouseNode(nextId++, "Warehouse", WAREHOUSE);
}

WarehouseTree::~WarehouseTree() { destroyTree(root); }

void WarehouseTree::destroyTree(WarehouseNode *node)
{
    if (!node)
        return;
    for (int i = 0; i < node->children.getSize(); i++)
        destroyTree(node->children[i]);
    delete node;
}

WarehouseNode *WarehouseTree::getRoot() { return root; }

WarehouseNode *WarehouseTree::findNodeById(WarehouseNode *node, int id)
{
    if (!node)
        return nullptr;
    if (node->id == id)
        return node;
    for (int i = 0; i < node->children.getSize(); i++)
    {
        WarehouseNode *found = findNodeById(node->children[i], id);
        if (found)
            return found;
    }
    return nullptr;
}

WarehouseNode *WarehouseTree::findById(int id)
{
    return findNodeById(root, id);
}

WarehouseNode *WarehouseTree::addZone(const string &name)
{
    if (!root)
        return nullptr;
    WarehouseNode *zone = new WarehouseNode(nextId++, name, ZONE, root);
    root->children.push_back(zone);
    return zone;
}

WarehouseNode *WarehouseTree::addAisle(int zoneId, const string &name)
{
    WarehouseNode *zone = findById(zoneId);
    if (!zone || zone->type != ZONE)
        return nullptr;
    WarehouseNode *aisle = new WarehouseNode(nextId++, name, AISLE, zone);
    zone->children.push_back(aisle);
    return aisle;
}

WarehouseNode *WarehouseTree::addShelf(int aisleId, const string &name)
{
    WarehouseNode *aisle = findById(aisleId);
    if (!aisle || aisle->type != AISLE)
        return nullptr;
    WarehouseNode *shelf = new WarehouseNode(nextId++, name, SHELF, aisle);
    aisle->children.push_back(shelf);
    return shelf;
}

bool WarehouseTree::removeNode(int id)
{
    if (!root || id == root->id)
        return false;
    WarehouseNode *node = findById(id);
    if (!node)
        return false;

    if (node->parent)
    {
        int idx = node->parent->children.find(node);
        if (idx >= 0)
            node->parent->children.remove_at(idx);
    }
    destroyTree(node);
    return true;
}

// Print tree with ASCII connectors
void WarehouseTree::printTree(WarehouseNode *node, string prefix, bool isLast)
{
    if (!node)
        return;

    cout << prefix;
    if (node == root)
        cout << node->name << " (ID: " << node->id << ")" << endl;
    else
    {
        cout << (isLast ? "+-- " : "|-- ");
        cout << node->name << " (ID: " << node->id << ")" << endl;
    }

    string newPrefix = prefix;
    if (node != root)
        newPrefix += (isLast ? "    " : "|   ");

    for (int i = 0; i < node->children.getSize(); i++)
        printTree(node->children[i], newPrefix, i == node->children.getSize() - 1);
}

void WarehouseTree::displayLayout()
{
    if (!root)
    {
        cout << "Empty warehouse." << endl;
        return;
    }
    printTree(root, "", true);
}

// Pre-order: visit node, then each child subtree
void WarehouseTree::preOrder(WarehouseNode *node)
{
    if (!node)
        return;
    cout << "  [" << node->id << "] " << node->name << endl;
    for (int i = 0; i < node->children.getSize(); i++)
        preOrder(node->children[i]);
}

void WarehouseTree::preOrderTraversal()
{
    cout << "Pre-Order Traversal:" << endl;
    preOrder(root);
}

// Level-order: visit level by level using a queue
void WarehouseTree::levelOrderTraversal()
{
    cout << "Level-Order Traversal:" << endl;
    if (!root)
        return;

    CustomQueue<WarehouseNode *> q;
    q.enqueue(root);

    while (!q.isEmpty())
    {
        WarehouseNode *node = q.dequeue();
        cout << "  [" << node->id << "] " << node->name << endl;
        for (int i = 0; i < node->children.getSize(); i++)
            q.enqueue(node->children[i]);
    }
}

void WarehouseTree::collectNodes(WarehouseNode *node, DynamicArray<WarehouseNode *> &list)
{
    if (!node)
        return;
    list.push_back(node);
    for (int i = 0; i < node->children.getSize(); i++)
        collectNodes(node->children[i], list);
}

DynamicArray<WarehouseNode *> WarehouseTree::getAllNodes()
{
    DynamicArray<WarehouseNode *> list;
    collectNodes(root, list);
    return list;
}

int WarehouseTree::getNodeCount()
{
    return getAllNodes().getSize();
}

// ============================================================
// WarehouseGraph
// ============================================================

void WarehouseGraph::addEdge(int src, int dest, int weight)
{
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES)
        return;
    if (!hasEdge(src, dest))
    {
        adjList[src].push_back(Edge(dest, weight));
        adjList[dest].push_back(Edge(src, weight));
    }
}

bool WarehouseGraph::hasEdge(int src, int dest)
{
    if (src < 0 || src >= MAX_NODES)
        return false;
    for (int i = 0; i < adjList[src].getSize(); i++)
        if (adjList[src][i].destId == dest)
            return true;
    return false;
}

void WarehouseGraph::clear()
{
    for (int i = 0; i < MAX_NODES; i++)
        adjList[i].clear();
}

// BFS shortest path - finds path with fewest hops
PathResult WarehouseGraph::findShortestPath(int src, int dest)
{
    PathResult result;
    if (src < 0 || src >= MAX_NODES || dest < 0 || dest >= MAX_NODES)
        return result;

    if (src == dest)
    {
        result.found = true;
        result.path.push_back(src);
        return result;
    }

    bool visited[MAX_NODES] = {false};
    int parent[MAX_NODES];
    int dist[MAX_NODES] = {0};
    for (int i = 0; i < MAX_NODES; i++)
        parent[i] = -1;

    CustomQueue<int> q;
    visited[src] = true;
    q.enqueue(src);

    bool found = false;
    while (!q.isEmpty() && !found)
    {
        int cur = q.dequeue();

        for (int i = 0; i < adjList[cur].getSize(); i++)
        {
            int next = adjList[cur][i].destId;
            int w = adjList[cur][i].weight;

            if (!visited[next])
            {
                visited[next] = true;
                parent[next] = cur;
                dist[next] = dist[cur] + w;

                if (next == dest)
                {
                    found = true;
                    break;
                }
                q.enqueue(next);
            }
        }
    }

    if (found)
    {
        result.found = true;
        result.totalCost = dist[dest];

        // Reconstruct path: walk parent chain backwards, use stack to reverse
        CustomStack<int> s;
        int cur = dest;
        while (cur != -1)
        {
            s.push(cur);
            cur = parent[cur];
        }
        while (!s.isEmpty())
            result.path.push_back(s.pop());
    }

    return result;
}

// BFS graph traversal - visits all reachable nodes level by level
DynamicArray<int> WarehouseGraph::bfsTraversal(int start)
{
    DynamicArray<int> order;
    if (start < 0 || start >= MAX_NODES)
        return order;

    bool visited[MAX_NODES] = {false};
    CustomQueue<int> q;
    visited[start] = true;
    q.enqueue(start);

    while (!q.isEmpty())
    {
        int cur = q.dequeue();
        order.push_back(cur);

        for (int i = 0; i < adjList[cur].getSize(); i++)
        {
            int next = adjList[cur][i].destId;
            if (!visited[next])
            {
                visited[next] = true;
                q.enqueue(next);
            }
        }
    }
    return order;
}

// DFS graph traversal using an explicit stack
DynamicArray<int> WarehouseGraph::dfsTraversal(int start)
{
    DynamicArray<int> order;
    if (start < 0 || start >= MAX_NODES)
        return order;

    bool visited[MAX_NODES] = {false};
    CustomStack<int> stk;
    stk.push(start);

    while (!stk.isEmpty())
    {
        int cur = stk.pop();
        if (visited[cur])
            continue;
        visited[cur] = true;
        order.push_back(cur);

        // Push neighbors in reverse so first neighbor is visited first
        DynamicArray<int> neighbors;
        for (int i = 0; i < adjList[cur].getSize(); i++)
            if (!visited[adjList[cur][i].destId])
                neighbors.push_back(adjList[cur][i].destId);
        for (int i = neighbors.getSize() - 1; i >= 0; i--)
            stk.push(neighbors[i]);
    }
    return order;
}

// Print adjacency list with location names
void WarehouseGraph::displayConnections(WarehouseTree &tree)
{
    DynamicArray<WarehouseNode *> nodes = tree.getAllNodes();

    for (int i = 0; i < nodes.getSize(); i++)
    {
        int id = nodes[i]->id;
        if (adjList[id].getSize() == 0)
            continue;

        cout << "  " << nodes[i]->name << " [" << id << "]:";
        for (int j = 0; j < adjList[id].getSize(); j++)
        {
            WarehouseNode *dest = tree.findById(adjList[id][j].destId);
            cout << " -> " << (dest ? dest->name : "?");
            cout << "(w:" << adjList[id][j].weight << ")";
        }
        cout << endl;
    }
}

// ============================================================
// Helper functions
// ============================================================

string getTypeName(LocationType type)
{
    switch (type)
    {
    case WAREHOUSE:
        return "WAREHOUSE";
    case ZONE:
        return "ZONE";
    case AISLE:
        return "AISLE";
    case SHELF:
        return "SHELF";
    default:
        return "?";
    }
}

// Builds the sample warehouse with 3 zones, 7 aisles, 16 shelves
void loadDefaultLayout(WarehouseTree &tree)
{
    tree.addZone("Zone A - Receiving"); // 1
    tree.addAisle(1, "Aisle A1");       // 2
    tree.addShelf(2, "Shelf A1-01");    // 3
    tree.addShelf(2, "Shelf A1-02");    // 4
    tree.addShelf(2, "Shelf A1-03");    // 5
    tree.addAisle(1, "Aisle A2");       // 6
    tree.addShelf(6, "Shelf A2-01");    // 7
    tree.addShelf(6, "Shelf A2-02");    // 8

    tree.addZone("Zone B - Storage"); // 9
    tree.addAisle(9, "Aisle B1");     // 10
    tree.addShelf(10, "Shelf B1-01"); // 11
    tree.addShelf(10, "Shelf B1-02"); // 12
    tree.addShelf(10, "Shelf B1-03"); // 13
    tree.addAisle(9, "Aisle B2");     // 14
    tree.addShelf(14, "Shelf B2-01"); // 15
    tree.addShelf(14, "Shelf B2-02"); // 16
    tree.addAisle(9, "Aisle B3");     // 17
    tree.addShelf(17, "Shelf B3-01"); // 18
    tree.addShelf(17, "Shelf B3-02"); // 19

    tree.addZone("Zone C - Dispatch"); // 20
    tree.addAisle(20, "Aisle C1");     // 21
    tree.addShelf(21, "Shelf C1-01");  // 22
    tree.addShelf(21, "Shelf C1-02");  // 23
    tree.addAisle(20, "Aisle C2");     // 24
    tree.addShelf(24, "Shelf C2-01");  // 25
    tree.addShelf(24, "Shelf C2-02");  // 26
}

// Builds the graph edges automatically from the tree structure
void buildGraph(WarehouseTree &tree, WarehouseGraph &graph)
{
    graph.clear();
    DynamicArray<WarehouseNode *> nodes = tree.getAllNodes();

    for (int i = 0; i < nodes.getSize(); i++)
    {
        WarehouseNode *node = nodes[i];

        // Parent-child edges with weights based on level
        if (node->parent)
        {
            int w = 1;
            if (node->type == ZONE)
                w = 3;
            else if (node->type == AISLE)
                w = 2;
            graph.addEdge(node->id, node->parent->id, w);
        }

        // Adjacent sibling edges (shelves in same aisle, aisles in same zone)
        for (int j = 0; j < node->children.getSize() - 1; j++)
            graph.addEdge(node->children[j]->id, node->children[j + 1]->id, 1);
    }
}

// ============================================================
// Main - menu-driven interface
// ============================================================

void runWarehouseMenu()
{
    WarehouseTree tree;
    WarehouseGraph graph;

    loadDefaultLayout(tree);
    buildGraph(tree, graph);

    int choice;

    while (true)
    {
        cout << "\n=== Warehouse Layout & Navigation ===" << endl;
        cout << " 1. Display Layout" << endl;
        cout << " 2. Display All Locations" << endl;
        cout << " 3. Add Zone" << endl;
        cout << " 4. Add Aisle" << endl;
        cout << " 5. Add Shelf" << endl;
        cout << " 6. Remove Location" << endl;
        cout << " 7. Find Shortest Path" << endl;
        cout << " 8. Pre-Order Traversal" << endl;
        cout << " 9. Level-Order Traversal" << endl;
        cout << "10. BFS Traversal (Graph)" << endl;
        cout << "11. DFS Traversal (Graph)" << endl;
        cout << "12. Show Graph Connections" << endl;
        cout << " 0. Exit" << endl;
        cout << "Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input." << endl;
            continue;
        }
        cin.ignore(10000, '\n');

        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            cout << endl;
            tree.displayLayout();
            break;

        case 2:
        {
            DynamicArray<WarehouseNode *> nodes = tree.getAllNodes();
            cout << "\nAll Locations:" << endl;
            for (int i = 0; i < nodes.getSize(); i++)
                cout << "  [" << nodes[i]->id << "] " << nodes[i]->name
                     << " (" << getTypeName(nodes[i]->type) << ")" << endl;
            break;
        }

        case 3:
        {
            string name;
            cout << "Enter zone name: ";
            getline(cin, name);
            WarehouseNode *z = tree.addZone(name);
            if (z)
            {
                buildGraph(tree, graph);
                cout << "Added '" << name << "' (ID: " << z->id << ")" << endl;
            }
            break;
        }

        case 4:
        {
            DynamicArray<WarehouseNode *> nodes = tree.getAllNodes();
            cout << "Available zones:" << endl;
            for (int i = 0; i < nodes.getSize(); i++)
                if (nodes[i]->type == ZONE)
                    cout << "  [" << nodes[i]->id << "] " << nodes[i]->name << endl;

            int zoneId;
            cout << "Zone ID: ";
            if (!(cin >> zoneId))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cin.ignore(10000, '\n');
            string name;
            cout << "Aisle name: ";
            getline(cin, name);

            WarehouseNode *a = tree.addAisle(zoneId, name);
            if (a)
            {
                buildGraph(tree, graph);
                cout << "Added '" << name << "' (ID: " << a->id << ")" << endl;
            }
            else
                cout << "Invalid zone ID." << endl;
            break;
        }

        case 5:
        {
            DynamicArray<WarehouseNode *> nodes = tree.getAllNodes();
            cout << "Available aisles:" << endl;
            for (int i = 0; i < nodes.getSize(); i++)
                if (nodes[i]->type == AISLE)
                    cout << "  [" << nodes[i]->id << "] " << nodes[i]->name << endl;

            int aisleId;
            cout << "Aisle ID: ";
            if (!(cin >> aisleId))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cin.ignore(10000, '\n');
            string name;
            cout << "Shelf name: ";
            getline(cin, name);

            WarehouseNode *s = tree.addShelf(aisleId, name);
            if (s)
            {
                buildGraph(tree, graph);
                cout << "Added '" << name << "' (ID: " << s->id << ")" << endl;
            }
            else
                cout << "Invalid aisle ID." << endl;
            break;
        }

        case 6:
        {
            int id;
            cout << "Location ID to remove: ";
            if (!(cin >> id))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cin.ignore(10000, '\n');

            WarehouseNode *node = tree.findById(id);
            if (!node)
            {
                cout << "Not found." << endl;
                break;
            }
            if (node->type == WAREHOUSE)
            {
                cout << "Cannot remove root." << endl;
                break;
            }

            string name = node->name;
            tree.removeNode(id);
            buildGraph(tree, graph);
            cout << "Removed '" << name << "'." << endl;
            break;
        }

        case 7:
        {
            int src, dest;
            cout << "Source ID: ";
            if (!(cin >> src))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cout << "Destination ID: ";
            if (!(cin >> dest))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cin.ignore(10000, '\n');

            WarehouseNode *srcNode = tree.findById(src);
            WarehouseNode *destNode = tree.findById(dest);
            if (!srcNode || !destNode)
            {
                cout << "Invalid ID." << endl;
                break;
            }

            PathResult res = graph.findShortestPath(src, dest);

            cout << "\nFrom: " << srcNode->name << " -> To: " << destNode->name << endl;
            if (res.found)
            {
                cout << "Path: ";
                for (int i = 0; i < res.path.getSize(); i++)
                {
                    if (i > 0)
                        cout << " -> ";
                    WarehouseNode *n = tree.findById(res.path[i]);
                    cout << (n ? n->name : "?");
                }
                cout << endl;
                cout << "Hops: " << res.path.getSize() - 1
                     << ", Distance: " << res.totalCost << endl;
            }
            else
            {
                cout << "No path found." << endl;
            }
            break;
        }

        case 8:
            tree.preOrderTraversal();
            break;

        case 9:
            tree.levelOrderTraversal();
            break;

        case 10:
        {
            int start;
            cout << "Start ID (0 for root): ";
            if (!(cin >> start))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cin.ignore(10000, '\n');

            DynamicArray<int> order = graph.bfsTraversal(start);
            WarehouseNode *startNode = tree.findById(start);
            cout << "\nBFS from " << (startNode ? startNode->name : "?") << ":" << endl;
            for (int i = 0; i < order.getSize(); i++)
            {
                WarehouseNode *n = tree.findById(order[i]);
                cout << "  " << (i + 1) << ". " << (n ? n->name : "?")
                     << " [" << order[i] << "]" << endl;
            }
            cout << "Visited: " << order.getSize() << " nodes" << endl;
            break;
        }

        case 11:
        {
            int start;
            cout << "Start ID (0 for root): ";
            if (!(cin >> start))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid." << endl;
                break;
            }
            cin.ignore(10000, '\n');

            DynamicArray<int> order = graph.dfsTraversal(start);
            WarehouseNode *startNode = tree.findById(start);
            cout << "\nDFS from " << (startNode ? startNode->name : "?") << ":" << endl;
            for (int i = 0; i < order.getSize(); i++)
            {
                WarehouseNode *n = tree.findById(order[i]);
                cout << "  " << (i + 1) << ". " << (n ? n->name : "?")
                     << " [" << order[i] << "]" << endl;
            }
            cout << "Visited: " << order.getSize() << " nodes" << endl;
            break;
        }

        case 12:
            cout << "\nGraph Connections:" << endl;
            graph.displayConnections(tree);
            break;

        default:
            cout << "Invalid choice." << endl;
        }
    }

    cout << "Returning to main menu." << endl;
}
