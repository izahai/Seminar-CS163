#pragma once
#include<vector>

using namespace std;

using Edge = pair<int, int>;

struct Graph {
	int numV;
	int numE;
	int timer;
	vector<vector<Edge>> listAdj;
	vector<int> low;
	vector<int> entry;
	vector<bool> listAP;
	vector<bool> visited;

	Graph(int v, int e);
	
	void loadArticulationPoints();
	void dfsTarjanAP(int cur, int prv);
	
	void loadAPNaiveDFS();
	void naiveDFS(int src, int rmv);

	void loadAPNaiveBFS();
	void naiveBFS(int src, int rmv);

	void loadAPCoutingChildDFS();
	int childDFS(int src);

	void printArticulationPoints(int choice);
	void resetContainer();
};	

