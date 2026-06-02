#include <iostream>
#include <string>
using namespace std;

namespace OrderManagement
{
    class order
    {
    public:
        int order_id;
        string customer_name;
        string product_name;

        order() {}
        order(int id, string customer, string product)
        {
            order_id = id;
            customer_name = customer;
            product_name = product;
        }
    };

    class Node
    {
    public:
        order data;
        Node *next;

        Node(order data)
        {
            this->data = data;
            next = nullptr;
        }
    };

    class Queue
    {
    private:
        Node *front;
        Node *rear;
        Node *completeFront;
        Node *completeRear;

        int currentSize;
        const int MAX_CAPACITY = 5; // Set low for easy testing

    public:
        Queue()
        {
            front = nullptr;
            rear = nullptr;
            completeFront = nullptr;
            completeRear = nullptr;
            currentSize = 0; // Start at 0
        }

        ~Queue() // Destructor to clean up memory
        {
            // Clean up pending orders
            Node *current = front;
            while (current != nullptr)
            {
                Node *nextNode = current->next;
                delete current;
                current = nextNode;
            }
            // Clean up completed history
            current = completeFront;
            while (current != nullptr)
            {
                Node *nextNode = current->next;
                delete current;
                current = nextNode;
            }
        }

        bool isEmpty()
        {
            return front == nullptr;
        }

        void enqueue(order o)
        {
            if (currentSize >= MAX_CAPACITY)
            {
                cout << "\n[!] SYSTEM OVERLOAD: Cannot accept more orders. Queue is full (Max " << MAX_CAPACITY << ")." << endl;
                return; // Stop the function here
            }

            Node *newNode = new Node(o);
            if (isEmpty())
            {
                front = newNode;
                rear = newNode;
            }
            else
            {
                rear->next = newNode;
                rear = newNode;
            }

            currentSize++;
            cout << "\n[+] Order successfully added to the queue!" << endl;
        }

        order dequeue()
        {
            if (isEmpty())
                return order(-1, "", "");

            Node *temp = front;
            order processedOrder = temp->data;

            front = front->next;
            if (front == nullptr)
                rear = nullptr;

            Node *newCompleted = new Node(processedOrder);

            if (completeFront == nullptr)
            {
                completeFront = completeRear = newCompleted;
            }
            else
            {
                completeRear->next = newCompleted;
                completeRear = newCompleted;
            }

            delete temp;
            currentSize--; // Decrease active order count

            return processedOrder;
        }

        void display()
        {
            if (isEmpty())
            {
                cout << "\n[!] No pending orders in the queue." << endl;
                return;
            }

            cout << "\n--- Pending Orders (" << currentSize << "/" << MAX_CAPACITY << ") ---" << endl;
            Node *current = front;
            while (current != nullptr)
            {
                cout << "Order ID: " << current->data.order_id
                     << " | Customer: " << current->data.customer_name
                     << " | Product: " << current->data.product_name << endl;
                current = current->next;
            }
            cout << "------------------------------" << endl;
        }

        void displayCompleted()
        {
            if (completeFront == nullptr)
            {
                cout << "\n[!] No orders have been completed yet." << endl;
                return;
            }

            cout << "\n--- Completed Orders ---" << endl;
            Node *current = completeFront;
            while (current != nullptr)
            {
                cout << "Order ID: " << current->data.order_id
                     << " | Customer: " << current->data.customer_name
                     << " | Product: " << current->data.product_name << endl;
                current = current->next;
            }
            cout << "------------------------" << endl;
        }
    };

    void runMenu()
    {
        Queue q;
        int choice;
        int idCounter = 1;

        do
        {
            cout << "\n=== ORDER MANAGEMENT MENU ===" << endl;
            cout << "1. Add Order" << endl;
            cout << "2. Process Order" << endl;
            cout << "3. View Pending Orders" << endl;
            cout << "4. View Completed Orders" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                cout << "\n[!] Invalid input. Please enter a number." << endl;
                cin.clear();
                while (cin.get() != '\n')
                    ;
                continue;
            }

            switch (choice)
            {
            case 1:
            {
                string name, product;
                cout << "Enter Customer Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Product Name: ";
                getline(cin, product);

                order newOrder(idCounter++, name, product);
                q.enqueue(newOrder);
                break;
            }
            case 2:
            {
                if (q.isEmpty())
                {
                    cout << "\n[!] Queue is empty. No orders to process." << endl;
                }
                else
                {
                    order processed = q.dequeue();

                    cout << "\nOrder ID: " << processed.order_id
                         << " has been assigned to a robot and removed from the pending list." << endl;
                }
                break;
            }
            case 3:
                q.display();
                break;
            case 4:
                q.displayCompleted();
                break;
            case 5:
                cout << "\nExiting Order Management System. Goodbye!" << endl;
                break;
            default:
                cout << "\n[!] Invalid choice. Please select from 1 to 5." << endl;
            }
        } while (choice != 5);
    }
}
