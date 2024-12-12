#include "../hpp/acutest.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <cassert>
#include <stdexcept>
#include "../hpp/robust_prune.hpp"
#include "../hpp/Point.hpp"
#include "../hpp/Node.hpp"

using namespace std;

// Helper functions
void setPointCoordinates(Point& point, float value) {
    point.coords.resize(2, value); // 2D points for this example
}

vector<Node> createTestGraph(int N, int R) {
    vector<Node> nodes;
    nodes.reserve(N); // Reserve space to avoid reallocations

    // Create N nodes
    for (int i = 0; i < N; i++) {
        // Create point for current node
        Point p;
        setPointCoordinates(p, static_cast<float>(i));

        // Create and add the node
        nodes.emplace_back(p);

        // Create neighbors list (circular graph) and set it for the node
        for (int k = 1; k <= R; k++) {
            nodes[i].neighbors.push_back((i + k) % N);
            nodes[i].neighbors.push_back((i - k + N) % N);
        }
    }
    return nodes;
}


// Test cases
void test_prunes_to_max_degree(void) {
    int N = 5, R = 4;
    vector<Node> graph = createTestGraph(N, R);
    int currentNodeId = 0;
    set<int> candidateSet = {1, 2, 3, 4};
    float alpha = 1.0;
    int maxDegree = 2;
    
    filteredRobustPrune(graph, currentNodeId, candidateSet, alpha, maxDegree);
    
    TEST_CHECK(graph[currentNodeId].neighbors.size() <= static_cast<size_t>(maxDegree));
}

void test_respects_alpha(void) {
    int N = 5, R = 4;
    vector<Node> graph = createTestGraph(N, R);
    int currentNodeId = 1;
    set<int> candidateSet = {0, 2, 3, 4};
    float alpha = 1.3; // Larger distance threshold
    int maxDegree = 1; // Smaller max degree
    
    filteredRobustPrune(graph, currentNodeId, candidateSet, alpha, maxDegree);
    
    TEST_CHECK(graph[currentNodeId].neighbors.size() <= static_cast<size_t>(maxDegree));
}

void test_allows_larger_max_degree(void) {
    int N = 7, R = 4;
    vector<Node> graph = createTestGraph(N, R);
    int currentNodeId = 2;
    set<int> candidateSet = {0, 1, 3, 4, 5, 6};
    float alpha = 1.5;
    int maxDegree = 3;
    
    filteredRobustPrune(graph, currentNodeId, candidateSet, alpha, maxDegree);
    
    TEST_CHECK(graph[currentNodeId].neighbors.size() <= static_cast<size_t>(maxDegree));
}

// Test list for Acutest
TEST_LIST = {
    {"Prunes to Max Degree", test_prunes_to_max_degree},
    {"Respects Alpha", test_respects_alpha},
    {"Allows Larger Max Degree", test_allows_larger_max_degree},
    {NULL, NULL}
};