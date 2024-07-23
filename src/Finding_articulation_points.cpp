#include <iostream>
#include <vector>

#include "articulation.h"

using namespace std;

int main()
{
    /*int v, e;
    cin >> v >> e;
    Graph graph(v, e);*/
    Graph graph = inputFromFile("input2.txt");
    graph.printAPnB(0);
    graph.printAPnB(1);
    graph.printAPnB(2);
    graph.printAPnB(3);
    return 0;
}

