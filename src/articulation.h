#pragma once
#include<vector>

using namespace std;

using Edge = pair<int, int>;

struct Graph {
	int numV;
	int numE;
	vector<vector<Edge>> listAdj;
	vector<int> low;
	vector<int> entry;
	vector<bool> listAP;
	vector<bool> visited;

	Graph(int v, int e);
	vector<bool> getArticulationPoints();
	void dfsAP(int cur, int prv);
	void printArticulationPoints();
};

