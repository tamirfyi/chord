#include <iostream>
#include "node.h"
#include "tlib.h"

int main()
{
    // Create nodes
    Node n0 = Node(0);
    Node n1 = Node(30);
    Node n2 = Node(65);
    Node n3 = Node(110);
    Node n4 = Node(160);
    Node n5 = Node(230);

    // Node joins
    n0.join(NULL);
    n1.join(&n0);
    n2.join(&n1);
    n3.join(&n2);
    n4.join(&n3);
    n5.join(&n4);

    // // Print Finger Tables - for debugging
    // n0.printInfo();
    // n1.printInfo();
    // n2.printInfo();
    // n3.printInfo();
    // n4.printInfo();
    // n5.printInfo();

    // Insert keys
    cout << "-> Inserting keys..." << endl;
    n0.insert(3, 3);
    n1.insert(200);
    n2.insert(123);
    n3.insert(45, 3);
    n4.insert(99);
    n2.insert(60, 10);
    n0.insert(50, 8);
    n3.insert(100, 5);
    n3.insert(101, 4);
    n3.insert(102, 6);
    n5.insert(240, 8);
    n5.insert(250, 10);

    // // Print Keys - for debugging
    // n0.printKeys();
    // n1.printKeys();
    // n2.printKeys();
    // n3.printKeys();
    // n4.printKeys();
    // n5.printKeys();

    // Join new node with ID = 100
    cout << "-> Node 100 joining network after Node 65" << endl;
    Node n6 = Node(100);
    n6.join(&n2);

    // Find all the keys
    int key_vals[12] = {3, 200, 123, 45, 99, 60, 50, 100, 101, 102, 240, 250};

    cout << "------------node 0------------" << endl;
    for (auto key : key_vals)
    {
        n0.find(key);
    }
    cout << endl;

    cout << "------------node 65------------" << endl;
    for (auto key : key_vals)
    {
        n2.find(key);
    }
    cout << endl;

    cout << "------------node 100------------" << endl;
    for (auto key : key_vals)
    {
        n6.find(key);
    }
    cout << endl;

    // Node(65) leaves the network
    n2.leave();
}