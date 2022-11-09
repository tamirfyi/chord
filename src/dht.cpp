#include <iostream>
#include "node.h"

int main()
{
    Node n0 = Node(30);
    // Node n1 = Node(30);

    n0.join(NULL);  // first node joining hash ring
    n0.printInfo(); // not first node
    // n1.join(&n0);
}