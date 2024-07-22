#include <iostream>
#include <vector>

#include "articulation.h"

using namespace std;

Graph::Graph(int v, int e) :
	listAdj(v),
	low(v),
	entry(v),
	listAP(v, false),
	visited(v, false)
{
	numE = e;
	numV = v;
	for (int i = 0; i < numE; ++i) {
		int x, y, w;
		cin >> x >> y >> w;
		listAdj[x].push_back(Edge(w, y));
		listAdj[y].push_back(Edge(w, x));
	}
}

void Graph::dfsAP(int cur, int prv) {
	visited[cur] = true;
	int numAdj = listAdj[cur].size();
	int childOfRoot = 0;
	for (int i = 0; i < numAdj; i++) {
		int nb = listAdj[cur][i].second;
		if (nb == prv)
			continue;
		if (!visited[i]) {
			dfsAP(nb, cur);
			low[cur] = min(low[cur], low[nb]);
			if (low[nb] >= entry[cur] && prv != -1)
				listAP[cur] = true;
			++childOfRoot;
		}
		else {
			low[cur] = min(low[cur], entry[nb]);
		}
	}
	if (prv == -1 && childOfRoot > 1)
		listAP[cur] = true;
	return;
}

vector<bool> Graph::getArticulationPoints() {
	for (int i = 0; i < numV; ++i) {
		if (!visited[i])
			dfsAP(i, -1);
	}
	return listAP;
}

void Graph::printArticulationPoints() {
	cout << "\nResult:\n";
	getArticulationPoints();
	int l = 0;
	for (int i = 0; i < numV; i++)
		if (listAP[i]) {
			cout << i << " is a articulation point\n";
			++l;
		}
	cout << "There are " << l << " articulation points in a graph\n";
	return;
}

