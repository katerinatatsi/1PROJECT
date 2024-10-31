#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>

using namespace std;

// Insert a directed edge to graph
void addEdge(vector<vector<int>>& graph, int u, int v);

// Check if edge u -> v exists in graph
bool edgeExists(vector<vector<int>> graph, int u, int v);

// Print the given graph
void printGraph(vector<vector<int>> graph);

// Create an R-regular directed graph with N nodes 
vector<vector<int>> createGraph(int N, int R);

#endif // GRAPH_HPP