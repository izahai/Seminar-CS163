#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>

#include "articulation.h"

using namespace std;
using namespace std::chrono;

Graph inputFromFile(string filename) {
	ifstream file(filename);
	int v, e;
	file >> v >> e;
	Graph graph(v, e);
	for (int i = 0; i < e; ++i) {
		int x, y, w;
		file >> x >> y >> w;
		graph.listAdj[x].push_back(Edge(w, y));
		graph.listAdj[y].push_back(Edge(w, x));
		graph.listEdge.push_back(Edge(x, y));
	}
	return graph;
}

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
}

void Graph::inputEdge() {
	for (int i = 0; i < numE; ++i) {
		int x, y, w;
		cin >> x >> y >> w;
		listAdj[x].push_back(Edge(w, y));
		listAdj[y].push_back(Edge(w, x));
		listEdge.push_back(Edge(x, y));
	}
}

void Graph::dfsTarjanAPnB(int cur, int prv) {
	visited[cur] = true;
	low[cur] = entry[cur] = timer++;
	int numAdj = listAdj[cur].size();
	int childOfRoot = 0;
	for (int i = 0; i < numAdj; i++) {
		int nb = listAdj[cur][i].second;
		if (nb == prv)
			continue;
		if (!visited[nb]) {
			dfsTarjanAPnB(nb, cur);
			low[cur] = min(low[cur], low[nb]);
			if (low[nb] >= entry[cur]) {
				if (prv != -1) 
					listAP[cur] = true;
				if (low[nb] > entry[cur]) 
					listBridge.push_back(Edge(cur, nb));
			}
			++childOfRoot;
		}
		else {
			low[cur] = min(low[cur], entry[nb]);
		}
	}
	if (prv == -1 && childOfRoot > 1) {
		listAP[cur] = true;
	}
	return;
}

void Graph::loadTarjanAPnB() {
	for (int i = 0; i < numV; ++i) {
		if (!visited[i])
			dfsTarjanAPnB(i, -1);
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

void Graph::naiveBridgeDFS(int src, int from, int to) {
	visited[src] = true;
	int numNb = listAdj[src].size();
	for (int i = 0; i < numNb; ++i) {
		int nb = listAdj[src][i].second;
		if ((src == from && nb == to) || (src == to && nb == from))
			continue;
		if (!visited[nb])
			naiveBridgeDFS(nb, from, to);
	}
}
void Graph::loadBridgeDFS() {
	for (int i = 0; i < numE; ++i) {
		fill(visited.begin(), visited.end(), false);
		int numSubGraph = 0;
		int u = listEdge[i].first;
		int v = listEdge[i].second;
		for (int j = 0; j < numV; ++j) {
			if (!visited[j]) {
				++numSubGraph;
				naiveBridgeDFS(j, u, v);
			}
		}
		if (numSubGraph > 1)
			listBridge.push_back(Edge(u, v));
	}
	return;
}

void Graph::loadBridgeBFS() {
	for (int i = 0; i < numE; ++i) {
		fill(visited.begin(), visited.end(), false);
		int numSubGraph = 0;
		int u = listEdge[i].first;
		int v = listEdge[i].second;
		for (int j = 0; j < numV; ++j) {
			if (!visited[j]) {
				++numSubGraph;
				naiveBridgeBFS(j, u, v);
			}
		}
		if (numSubGraph > 1)
			listBridge.push_back(Edge(u, v));
	}
	return;
}
void Graph::naiveBridgeBFS(int src, int from, int to) {
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
			if ((cur == from && to == nb) || (cur == to && nb == from))
				continue;
			if (!visited[nb])
				q.push(nb);
		}
	}
	return;
}

void Graph::printAPnB(int choice) {
	resetContainer();
	high_resolution_clock::time_point t1, t2;
	if (choice == 0) {
		cout << "\nResult for Tarjan's algorithm:\n";
		t1 = high_resolution_clock::now();
		loadTarjanAPnB();
		t2 = high_resolution_clock::now();
	}
	else if (choice == 1) {
		cout << "\nResult for removing naive DFS algorithm:\n";
		t1 = high_resolution_clock::now();
		loadAPNaiveDFS();
		loadBridgeDFS();
		t2 = high_resolution_clock::now();
	}
	else if (choice == 2) {
		cout << "\nResult for removing naive BFS algorithm:\n";
		t1 = high_resolution_clock::now();
		loadAPNaiveBFS();
		loadBridgeBFS();
		t2 = high_resolution_clock::now();
	}
	else if (choice == 3) {
		cout << "\nResult for counting child DFS algorithm:\n";
		t1 = high_resolution_clock::now();
		loadAPCoutingChildDFS();
		t2 = high_resolution_clock::now();
	}
	else {
		cout << "Invalid choice\n";
		return;
	}
	
	int l = 0;
	for (int i = 0; i < numV; i++)
		if (listAP[i]) {
			cout << i << " ";
			++l;
		}
	cout << "are articulation points\n";
	for (int i = 0; i < listBridge.size(); i++) 
		cout << "(" << listBridge[i].first << "-" << listBridge[i].second << ") ";
	if (listBridge.size() > 0) cout << "are bridges\n";
	cout << "There are " << l << " articulation points in a graph\n";
	cout << "It's took " << (duration_cast<duration<double>>(t2 - t1)).count() * 1000 << " ms\n";
	return;
}

void Graph::resetContainer() {
	timer = 0;
	fill(listAP.begin(), listAP.end(), false);
	fill(visited.begin(), visited.end(), false);
	listBridge.clear();
}




