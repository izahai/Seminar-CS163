#include <iostream>
#include <vector>

#include "articulation.h"

using namespace std;

int main()
{
    int v, e;
    cin >> v >> e;
    Graph graph(v, e);
    graph.printArticulationPoints();
    return 0;
}

