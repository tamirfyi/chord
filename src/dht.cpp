#include <iostream>
#include "node.h"
#include "tlib.h"

int main()
{
    Node n0 = Node(0);
    Node n1 = Node(30);
    Node n2 = Node(65);
    Node n3 = Node(110);
    Node n4 = Node(160);
    Node n5 = Node(230);

    // Node joins
    n0.join(NULL);
    n1.join(&n0);
    n2.join(&n0);
    n3.join(&n2);
    n4.join(&n3);
    n5.join(&n4);

    // Node Finger Tables
    n0.printInfo();
    cout << endl;
    n1.printInfo();
    cout << endl;
    n2.printInfo();
    cout << endl;
    n3.printInfo();
    cout << endl;
    n4.printInfo();
    cout << endl;
    n5.printInfo();
    cout << endl;

    // Key Inserts
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

    n0.printKeys();
    cout << endl;
    n1.printKeys();
    cout << endl;
    n2.printKeys();
    cout << endl;
    n3.printKeys();
    cout << endl;
    n4.printKeys();
    cout << endl;
    n5.printKeys();
    cout << endl;

    Node n6 = Node(100);
    n6.join(&n2);
    cout << endl;

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

    n2.leave();
    n0.printInfo();
    cout << endl;
    n1.printInfo();
    cout << endl;
    n0.printKeys();
    cout << endl;
    n1.printKeys();
    cout << endl;
    n6.printKeys();
    cout << endl;
}