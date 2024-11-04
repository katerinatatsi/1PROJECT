#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <random>

#include "../hpp/vamana_indexing.hpp"
#include "../hpp/greedy_search.hpp"
#include "../hpp/robust_prune.hpp"

using namespace std;

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

vector<int> generateRandomPermutation(int N) {
    // cout << "Generating random permutation..." << endl;

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

vector<Node> vamana_indexing(const vector<Point>& points, int L, int R) {
    const float alpha = 1.15;

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