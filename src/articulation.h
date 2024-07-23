#pragma once
#include<vector>
#include<string>
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
	vector<Edge> listBridge;
	vector<bool> visited;
	vector<Edge> listEdge;

	Graph(int v, int e);
	
	void loadTarjanAPnB();
	void dfsTarjanAPnB(int cur, int prv);
	
	void loadAPNaiveDFS();
	void naiveDFS(int src, int rmv);

	void loadAPNaiveBFS();
	void naiveBFS(int src, int rmv);

	void loadAPCoutingChildDFS();
	int childDFS(int src);

	void loadBridgeDFS();
	void naiveBridgeDFS(int src, int from, int to);

	void loadBridgeBFS();
	void naiveBridgeBFS(int src, int from, int to);

	void inputEdge();
	void printAPnB(int choice);
	void resetContainer();
};	

Graph inputFromFile(string filename);