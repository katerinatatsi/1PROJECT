#include "../hpp/acutest.hpp"
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cstring> 
#include <vector>
#include <cmath> 
#include <iomanip>
#include <map>
#include "../hpp/greedy_search.hpp"
#include "../hpp/Point.hpp"
#include "../hpp/Node.hpp"

using namespace std;

// Constants for test setup
const int N = 10;  // Number of nodes
const int D = 3;   // Dimension of points

// Helper functions
void setPointCoordinates(Point& point, const vector<float>& values) {
    point.coords = values;
}

vector<Node> createTestGraph(int N) {
    vector<Node> graph;
    graph.reserve(N);
    
    // Create N nodes with unique coordinates
    for (int i = 0; i < N; i++) {
        // Create point for current node with slightly different coordinates
        Point p;
        vector<float> coords = {
            static_cast<float>(i), 
            static_cast<float>(i * 0.5), 
            static_cast<float>(i * 1.5)
        };
        setPointCoordinates(p, coords);
        p.category = -1;  // Unfiltered category
        
        // Create Node
        Node node(p);
        
        // Create neighbors list (circular graph)
        for (int k = 1; k <= 2; k++) {
            node.neighbors.push_back((i + k) % N);
            node.neighbors.push_back((i - k + N) % N);
        }
        
        // Add the node to the graph
        graph.push_back(node);
    }
    
    return graph;
}

// Create a map for filtered search
map<int, int> createFilteredMap(int N) {
    map<int, int> M;
    for (int i = 0; i < N; i++) {
        M[i] = i;  // Map each index to itself
    }
    return M;
}

// Test cases
void test_finds_starting_node() {
    vector<Node> graph = createTestGraph(N);
    map<int, int> M = createFilteredMap(N);
    
    Point query;
    vector<float> queryCoords = {0.0f, 0.0f, 0.0f};
    setPointCoordinates(query, queryCoords);
    query.category = -1;  // Unfiltered query

    auto [result, visited] = filteredGreedySearch(graph, M, query, 1, 2);
    
    TEST_CHECK(result.empty() == false);
}

void test_finds_k_nearest() {
    vector<Node> graph = createTestGraph(N);
    map<int, int> M = createFilteredMap(N);
    
    Point query;
    vector<float> queryCoords = {0.0f, 0.0f, 0.0f};
    setPointCoordinates(query, queryCoords);
    query.category = -1;  // Unfiltered query

    auto [result, visited] = filteredGreedySearch(graph, M, query, 3, 4);
    
    TEST_CHECK(result.size() == 3);
    
    // Check if results are sorted by distance
    for (size_t i = 1; i < result.size(); i++) {
        float dist1 = graph[result[i-1]].point - query;
        float dist2 = graph[result[i]].point - query;
        TEST_CHECK(dist1 <= dist2);
    }
}

void test_respects_list_size() {
    vector<Node> graph = createTestGraph(N);
    map<int, int> M = createFilteredMap(N);
    
    Point query;
    vector<float> queryCoords = {0.0f, 0.0f, 0.0f};
    setPointCoordinates(query, queryCoords);
    query.category = -1;  // Unfiltered query

    auto [result, visited] = filteredGreedySearch(graph, M, query, N, 2);
    
    TEST_CHECK(visited.size() <= static_cast<size_t>(N));
    TEST_CHECK(result.size() <= static_cast<size_t>(N));
}

void test_edge_cases() {
    vector<Node> graph = createTestGraph(N);
    map<int, int> M = createFilteredMap(N);
    
    Point query;
    vector<float> queryCoords = {0.0f, 0.0f, 0.0f};
    setPointCoordinates(query, queryCoords);
    query.category = -1;  // Unfiltered query
    
    auto [result1, visited1] = filteredGreedySearch(graph, M, query, 0, 2);
    TEST_CHECK(result1.empty());

    auto [result2, visited2] = filteredGreedySearch(graph, M, query, 2, 1);
    TEST_CHECK(visited2.size() <= static_cast<size_t>(N));
}

void test_identical_points() {
    // Create nodes with nearly identical points
    vector<Node> graph;
    graph.reserve(N);
    
    for (int i = 0; i < N; i++) {
        Point p;
        vector<float> coords = {1.0f, 1.0f, 1.0f};
        setPointCoordinates(p, coords);
        p.category = -1;  // Unfiltered category
        
        Node node(p);
        
        vector<int> neighbors;
        neighbors.reserve(4);
        for (int k = 1; k <= 2; k++) {
            neighbors.push_back((i + k) % N);
            neighbors.push_back((i - k + N) % N);
        }
        node.neighbors = move(neighbors);
        
        graph.push_back(move(node));
    }
    
    map<int, int> M = createFilteredMap(N);
    
    Point query;
    vector<float> queryCoords = {1.0f, 1.0f, 1.0f};
    setPointCoordinates(query, queryCoords);
    query.category = -1;  // Unfiltered query

    auto [result, visited] = filteredGreedySearch(graph, M, query, 3, 3);
    
    TEST_CHECK(result.size() == 3);
    
    for (size_t i = 1; i < result.size(); i++) {
        float dist1 = graph[result[0]].point - query;
        float dist2 = graph[result[i]].point - query;
        TEST_CHECK(abs(dist1 - dist2) < 1e-6f);
    }
}

TEST_LIST = {
    {"Find Starting Node", test_finds_starting_node},
    {"Find K Nearest Nodes", test_finds_k_nearest},
    {"Respect List Size", test_respects_list_size},
    {"Edge Cases", test_edge_cases},
    {"Identical Points", test_identical_points},
    {NULL, NULL}
};