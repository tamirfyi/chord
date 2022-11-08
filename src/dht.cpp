#include <iostream>
#include "node.h"

int main()
{
    Node n0 = Node(0);
    Node n1 = Node(30);

    n0.join(NULL);

    std::cout << "Successor " << n0.getSuccessor() << std::endl;
    std::cout << "Predecessor " << n0.getPredecessor() << std::endl;
}