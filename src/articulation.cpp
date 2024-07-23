#include <iostream>
#include <vector>
#include <queue>
#include <chrono>

#include "articulation.h"

using namespace std;
using namespace std::chrono;

Graph::Graph(int v, int e) :
	listAdj(v),
	low(v),
	entry(v),
	listAP(v, false),
	visited(v, false)
{
	numE = e;
	numV = v;
	timer = 0;
	for (int i = 0; i < numE; ++i) {
		int x, y, w;
		cin >> x >> y >> w;
		listAdj[x].push_back(Edge(w, y));
		listAdj[y].push_back(Edge(w, x));
	}
}

void Graph::dfsTarjanAP(int cur, int prv) {
	visited[cur] = true;
	low[cur] = entry[cur] = timer++;
	int numAdj = listAdj[cur].size();
	int childOfRoot = 0;
	for (int i = 0; i < numAdj; i++) {
		int nb = listAdj[cur][i].second;
		if (nb == prv)
			continue;
		if (!visited[nb]) {
			dfsTarjanAP(nb, cur);
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

void Graph::loadArticulationPoints() {
	for (int i = 0; i < numV; ++i) {
		if (!visited[i])
			dfsTarjanAP(i, -1);
	}
}

void Graph::naiveDFS(int src, int rmv) {
	visited[src] = true;
	int numNb = listAdj[src].size();
	for (int i = 0; i < numNb; ++i) {
		int nb = listAdj[src][i].second;
		if (nb != rmv && !visited[nb])
			naiveDFS(nb, rmv);
	}
	return;
}

void Graph::loadAPNaiveDFS() {
	for (int i = 0; i < numV; ++i) {
		int numSubGraph = 0;
		for (int j = 0; j < numV; ++j) {
			if (j != i && !visited[j]) {
				++numSubGraph;
				naiveDFS(j, i);
			}
		}
		fill(visited.begin(), visited.end(), false);
		if (numSubGraph > 1)
			listAP[i] = true;
	}
	return;
}

void Graph::naiveBFS(int src, int rmv) {
	queue<int> q;
	q.push(src);
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		if (visited[cur])
			continue;
		visited[cur] = true;
		int numNb = listAdj[cur].size();
		for (int i = 0; i < numNb; ++i) {
			int nb = listAdj[cur][i].second;
			if (nb != rmv && !visited[nb])
				q.push(nb);
		}
	}
	return;
}

void Graph::loadAPNaiveBFS() {
	for (int i = 0; i < numV; ++i) {
		int numSubGraph = 0;
		for (int j = 0; j < numV; ++j) {
			if (j != i && !visited[j]) {
				++numSubGraph;
				naiveBFS(j, i);
			}
		}
		fill(visited.begin(), visited.end(), false);
		if (numSubGraph > 1)
			listAP[i] = true;
	}
	return;
}

int Graph::childDFS(int src) {
	visited[src] = true;
	int numNb = listAdj[src].size();
	int numChild = 0;
	for (int i = 0; i < numNb; ++i) {
		int nb = listAdj[src][i].second;
		if (!visited[nb]) {
			childDFS(nb);
			++numChild;
		}
	}
	return numChild;
}

void Graph::loadAPCoutingChildDFS() {
	int numChild;
	for (int i = 0; i < numV; ++i) {
		numChild = 0;
		numChild = childDFS(i);
		fill(visited.begin(), visited.end(), false);
		if (numChild > 1)
			listAP[i] = true;
	}
	return;
}

void Graph::printArticulationPoints(int choice) {
	resetContainer();
	high_resolution_clock::time_point t1, t2;
	if (choice == 0) {
		cout << "\nResult for Tarjan's algorithm:\n";
		t1 = high_resolution_clock::now();
		loadArticulationPoints();
		t2 = high_resolution_clock::now();
	}
	else if (choice == 1) {
		cout << "\nResult for removing naive DFS algorithm:\n";
		t1 = high_resolution_clock::now();
		loadAPNaiveDFS();
		t2 = high_resolution_clock::now();
	}
	else if (choice == 2) {
		cout << "\nResult for removing naive BFS algorithm:\n";
		t1 = high_resolution_clock::now();
		loadAPNaiveBFS();
		t2 = high_resolution_clock::now();
	}
	else if (choice == 3) {
		cout << "\nResult for removing naive counting children DFS algorithm:\n";
		t1 = high_resolution_clock::now();
		loadAPCoutingChildDFS();
		t2 = high_resolution_clock::now();
	}
	
	int l = 0;
	for (int i = 0; i < numV; i++)
		if (listAP[i]) {
			cout << i << " is a articulation point\n";
			++l;
		}
	cout << "There are " << l << " articulation points in a graph\n";
	cout << "It's took " << (duration_cast<duration<double>>(t2 - t1)).count() * 1000 << " ms\n";
	return;
}

void Graph::resetContainer() {
	timer = 0;
	fill(listAP.begin(), listAP.end(), false);
	fill(visited.begin(), visited.end(), false);
}

