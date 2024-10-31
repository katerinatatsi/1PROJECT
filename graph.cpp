#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

void addEdge(vector<vector<int>>& graph, int u, int v) {
    graph[u].push_back(v); // Add v to u's list (only one direction)
}

bool edgeExists(vector<vector<int>> graph, int u, int v) {
    // Check if v exists in u's adjacency list
    return find(graph[u].begin(), graph[u].end(), v) != graph[u].end();
}

void printGraph(vector<vector<int>> graph) {
    for (int i = 0; i < (int)graph.size(); ++i) {
        cout << "Node " << i << " has edges to: ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << "\n";
    }
}

vector<vector<int>> createGraph(int N, int R) {
    // Represent graph as an adjacency list
    vector<vector<int>> graph(N);
    int outDegree[N] = {0};

    // Create a graph with circular format where each node has exactly R outgoing edges 
    for (int i = 0; i < N; i++) {
        for (int k = 1; k <= R; k++)
            // Check if number of neighbors is less than R
            if (outDegree[i] < R) {
                int neighbor = (i + k) % N; // Calculate the neighbor node
                if (!edgeExists(graph, i, neighbor)) {
                    addEdge(graph, i, neighbor); // Add the corresponding edge
                    outDegree[i]++;              // Update out-degree
                }
            }
    }

    return graph;
}