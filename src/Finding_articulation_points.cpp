#include <iostream>
#include <vector>

#include "articulation.h"

using namespace std;

int main()
{
    int v, e;
    cin >> v >> e;
    Graph graph(v, e);
    graph.printArticulationPoints(0);
    graph.printArticulationPoints(1);
    graph.printArticulationPoints(2);
    graph.printArticulationPoints(3);
    return 0;
}

