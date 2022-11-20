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

    n0.join(NULL); // first node joining hash ring
    n1.join(&n0);
    n2.join(&n1);
    n3.join(&n2);
    n4.join(&n3);
    n5.join(&n4);

    // n0.printInfo(); // not first node
    // n1.printInfo();
    // n2.printInfo();
    // n3.printInfo();
    // n4.printInfo();
    // n5.printInfo();

    n0.insert(20, 1);
    n1.printKeys();
}