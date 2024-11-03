#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "../hpp/vamana_indexing.hpp"

using namespace std;

vector<Node> initializeGraph(const vector<Point>& points, int R) {
    const int N = points.size();
    vector<Node> graph(N);
    int outDegree[N] = {0};

    // Create a graph with circular format where each node has exactly R outgoing edges 
    for (int i = 0; i < N; i++) {
        vector<int> neighbors;
        for (int k = 1; k <= R; k++) {
            int neighbor = (i + k) % N;    // Calculate the neighbor node
            neighbors.push_back(neighbor); // Add the corresponding edge
            outDegree[i]++;                // Update out-degree
        }
        graph.push_back(Node(points[i], neighbors));
    }

    return graph;
}

int findMedoidId(const vector<Point>& points) {
    double minTotalDistance = numeric_limits<double>::infinity();
    int medoidIndex = -1;

    for (int i = 0; i < (int)points.size(); i++) {
        double totalDistance = 0.0;
        for (int j = 0; j < (int)points.size(); j++)
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
    // Step 1: Initialize a random R-regular directed graph
    vector<Node> graph = initializeGraph(points, R);

    // Step 2: Find the medoid of dataset
    int medoidIndex = findMedoidId(points);

    // Step 3: Randomly permute the numbers {0, 1, ..., N-1}
    const int N = (int)points.size();
    vector<int> randomPermuation = generateRandomPermutation(N);

    /*
        To be completed...
    */
}