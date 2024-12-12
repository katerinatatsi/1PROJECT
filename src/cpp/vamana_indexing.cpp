#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <random>
#include <map>

#include "../hpp/vamana_indexing.hpp"
#include "../hpp/greedy_search.hpp"
#include "../hpp/robust_prune.hpp"
#include "../hpp/medoid.hpp"

using namespace std;

// Create an empty graph that contains only nodes with no edges
vector<Node> createEmptyGraph(const vector<Point>& points) {
    // cout << "Creating the graph..." << endl;

    // cout << points.size() << endl;

    vector<Node> graph;
    vector<int> neighbors;

    for (int i = 0; i < (int)points.size(); i++) {
        Node node(points[i]);
        graph.push_back(node);
    }

    //cout << graph.size() << endl;

    return graph;
}

vector<int> generateRandomPermutation(int N) {
    // Create a vector with numbers 0 to N-1
    vector<int> numbers(N);
    for (int i = 0; i < N; ++i) {
        numbers[i] = i;
    }

    // Seed and create a random number generator
    random_device rd;  // Seed generator
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    // Shuffle the numbers to get a random permutation
    shuffle(numbers.begin(), numbers.end(), gen);

    return numbers;
}

vector<Node> filteredVamanaIndexing(const vector<Point>& points, float alpha, int L, int R) {
    // cout << "Running Filtered Vamana Indexing..." << endl;

    // Step 1: Initialize G to an empty graph
    vector<Node> graph = createEmptyGraph(points);

    // Step 2: Find the medoid mapping
    map<int, int> M = findMedoid(points, 1);

    // Step 3: Randomly permute the numbers {0, 1, ..., N-1}
    vector<int> randomPermuation = generateRandomPermutation((int)points.size());

    for (int currentId: randomPermuation) {
        // Step 4: Run greedy search
        pair<vector<int>, set<int>> res = filteredGreedySearch(graph, M, points[currentId], 1, L);
        set<int> candidates = res.second;

        // Step 5: Run robust prune to update out-neighbors of point with id = currentId
        filteredRobustPrune(graph, currentId, candidates, alpha, R);

        // Step 6: Update out-neighbors of point with id = neighborId
        for (int neighborId: graph[currentId].neighbors) {
            graph[neighborId].neighbors.push_back(currentId);

            if ((int)graph[neighborId].neighbors.size() > R) {
                // Convert back to set for robust prune
                set<int> prunedNeighbors(graph[neighborId].neighbors.begin(), graph[neighborId].neighbors.end());
                filteredRobustPrune(graph, neighborId, prunedNeighbors, alpha, R);
            }
        }
    }

    return graph;
}

vector<Node> initializeGraph(const vector<Point>& points, int R) {
    // cout << "Initializing graph..." << endl;

    const int N = points.size();
    vector<Node> graph;
    int outDegree[N] = {0};

    // Create a graph with circular format where each node has exactly R outgoing edges
    vector<int> neighbors(R);
    for (int i = 0; i < N; i++) {
        neighbors.clear();
        for (int k = 1; k <= R; k++) {
            int neighbor = (i + k) % N;    // Calculate the neighbor node
            neighbors.push_back(neighbor); // Add the corresponding edge
            outDegree[i]++;                // Update out-degree
        }
        Node node(points[i], neighbors);
        graph.push_back(node);
    }

    return graph;
}

int findMedoidId(const vector<Point>& points) {
    // cout << "Finding medoid point..." << endl;

    double minTotalDistance = numeric_limits<double>::infinity();
    int medoidIndex = -1;

    for (size_t i = 0; i < points.size(); i++) {
        double totalDistance = 0.0;
        for (size_t j = 0; j < points.size(); j++)
            if (i != j) 
                totalDistance += points[i] - points[j];
            
        if (totalDistance < minTotalDistance) {
            minTotalDistance = totalDistance;
            medoidIndex = i;
        }
    }

    return medoidIndex;
}

vector<Node> vamanaIndexing(const vector<Point>& points, float alpha, int L, int R) {
    // Step 1: Initialize a random R-regular directed graph
    vector<Node> graph = initializeGraph(points, R);

    // Step 2: Find the medoid of dataset
    int medoidId = findMedoidId(points);

    // Step 3: Randomly permute the numbers {0, 1, ..., N-1}
    const int N = (int)points.size();
    vector<int> randomPermuation = generateRandomPermutation(N);

    for (int currentId: randomPermuation) {
        // Step 5: Run greedy search
        pair<vector<int>, set<int>> res = greedySearch(graph, medoidId, points[currentId], 1, L);
        vector<int> kNearestNeighbors = res.first;
        set<int> candidates = res.second;

        // Step 6: Run robust prune to update out-neighbors of point with id = currentId
        robustPrune(graph, currentId, candidates, alpha, R);

        // Step 7: Update out-neighbors of point with id = neighborId
        for (int neighborId: graph[currentId].neighbors) {
            unordered_set<int> newNeighbors(graph[neighborId].neighbors.begin(), graph[neighborId].neighbors.end());
            newNeighbors.insert(currentId);

            if (newNeighbors.size() > R) {
                // Convert back to set for robust prune
                set<int> prunedNeighbors(newNeighbors.begin(), newNeighbors.end());
                robustPrune(graph, neighborId, prunedNeighbors, alpha, R);
            }
            else if (find(graph[neighborId].neighbors.begin(), graph[neighborId].neighbors.end(), currentId) == graph[neighborId].neighbors.end()) {
                // Only add if currentId is not already a neighbor
                graph[neighborId].neighbors.push_back(currentId);
            }
        }
    }

    return graph;
}

void stitchGraphs(vector<Node>& G, const vector<Node>& Gf, map<int, int> M) {
    for (int i = 0; i < (int)Gf.size(); i++) {
        int global_i = M[i];
        for (int neighbor: Gf[i].neighbors) {
            int global_neighbor = M[neighbor];

            if (find(G[global_i].neighbors.begin(), G[global_i].neighbors.end(), global_neighbor) == G[global_i].neighbors.end()) {
                G[global_i].neighbors.push_back(global_neighbor);
            }
        }
    }
}

vector<Node> stitchedVamanaIndexing(const vector<Point>& points, set<int> filters, float alpha, int L_small, int R_small, int R_stitched) {
    vector<Node> G = createEmptyGraph(points);
    // Create Pf: list with all ids corresponding to filter f
    vector<Point> Pf;
    map<int, int> M;

    for (int f: filters) {
        M.clear();
        Pf.clear();

        for (int i = 0; i < (int)points.size(); i++) {
            if (points[i].category == f) {
                Pf.push_back(points[i]);
                M[(int)Pf.size() - 1] = i;
            }
        }
        
        // vector<Node> Gf = vamanaIndexing(Pf, alpha, L_small, R_small);
        vector<Node> Gf = vamanaIndexing(Pf, alpha, L_small, R_small);
        stitchGraphs(G, Gf, M);
    }

    for (int i = 0; i < (int)points.size(); i++) {
        set<int> neighborsSet;
        neighborsSet.insert(G[i].neighbors.begin(), G[i].neighbors.end());

        filteredRobustPrune(G, i, neighborsSet, alpha, R_stitched);
    }

    return G;

}
