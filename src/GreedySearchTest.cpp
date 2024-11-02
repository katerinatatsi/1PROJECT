#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cstring> 
#include <vector>
#include <cmath> 
#include "hpp/GreedySearch.hpp"
#include "hpp/Point.hpp"
using namespace std;

// Constants for test setup
const int N = 10;  // Number of nodes
const int D = 3;   // Dimension of points

// Helper functions
void setPointCoordinates(Point& point, float value) {
    point.coords.resize(D, value);
}

void initializeNodes(vector<Node>& nodes, int N) {
    nodes.resize(N);
    for (int i = 0; i < N; i++) {
        // Initialize point coordinates
        nodes[i].position.coords.resize(D);
        for (int j = 0; j < D; j++) {
            nodes[i].position.coords[j] = static_cast<float>(i + j);
        }
        
        // Add neighbors (create a simple circular graph)
        for (int k = 1; k <= 2; k++) {
            nodes[i].neighbors.push_back((i + k) % N);
            nodes[i].neighbors.push_back((i - k + N) % N);
        }
    }
}

// Simple test framework macros
#define TEST_CASE(name) void name()
#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + #a + " != " + #b); \
    } \
} while(0)

#define ASSERT_LE(a, b) do { \
    if ((a) > (b)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + #a + " > " + #b); \
    } \
} while(0)

#define ASSERT_TRUE(condition) do { \
    if (!(condition)) { \
        throw std::runtime_error(std::string("Assertion failed: ") + #condition); \
    } \
} while(0)

#define RUN_TEST(test) do { \
    std::cout << "Running " << #test << "... "; \
    try { \
        test(); \
        std::cout << "PASSED" << std::endl; \
    } catch (const std::exception& e) { \
        std::cout << "FAILED: " << e.what() << std::endl; \
    } \
} while(0)

// Helper distance function
float euclidean_distance(const Point& p1, const Point& p2) {
    return p1 - p2;  // Using the overloaded - operator from Point.cpp
}

// Test cases
TEST_CASE(test_finds_starting_node) {
    vector<Node> nodes;
    initializeNodes(nodes, N);
    
    Point query;
    setPointCoordinates(query, 0.0);

    auto [result, visited] = greedySearch(nodes, 0, query, 1, 2);
    
    ASSERT_TRUE(!result.empty());
    ASSERT_EQ(result[0], 0);
}

TEST_CASE(test_finds_k_nearest) {
    vector<Node> nodes;
    initializeNodes(nodes, N);
    
    Point query;
    setPointCoordinates(query, 0.0);

    auto [result, visited] = greedySearch(nodes, 0, query, 3, 4);
    
    ASSERT_EQ(result.size(), 3);
    
    // Check if results are sorted by distance
    for (size_t i = 1; i < result.size(); i++) {
        ASSERT_TRUE(
            euclidean_distance(nodes[result[i-1]].position, query) <=
            euclidean_distance(nodes[result[i]].position, query)
        );
    }
}

TEST_CASE(test_respects_list_size) {
    vector<Node> nodes;
    initializeNodes(nodes, N);
    
    Point query;
    setPointCoordinates(query, 0.0);

    auto [result, visited] = greedySearch(nodes, 0, query, N, 2);
    
    ASSERT_LE(visited.size(), N);
    ASSERT_LE(result.size(), N);
}

TEST_CASE(test_edge_cases) {
    vector<Node> nodes;
    initializeNodes(nodes, N);
    
    Point query;
    setPointCoordinates(query, 0.0);
    
    auto [result1, visited1] = greedySearch(nodes, 0, query, 0, 2);
    ASSERT_TRUE(result1.empty());

    auto [result2, visited2] = greedySearch(nodes, 0, query, 2, 1);
    ASSERT_LE(visited2.size(), N);
}

TEST_CASE(test_identical_points) {
    vector<Node> nodes;
    initializeNodes(nodes, N);
    
    // Make all points identical
    for (auto& node : nodes) {
        node.position.coords.assign(D, 1.0);
    }
    
    Point query;
    setPointCoordinates(query, 1.0);

    auto [result, visited] = greedySearch(nodes, 0, query, 3, 3);
    
    ASSERT_EQ(result.size(), 3);
    
    for (size_t i = 1; i < result.size(); i++) {
        float dist1 = euclidean_distance(nodes[result[0]].position, query);
        float dist2 = euclidean_distance(nodes[result[i]].position, query);
        ASSERT_TRUE(abs(dist1 - dist2) < 1e-6);
    }
}

int main() {
    std::cout << "Running Greedy Search Tests...\n\n";
    
    RUN_TEST(test_finds_starting_node);
    RUN_TEST(test_finds_k_nearest);
    RUN_TEST(test_respects_list_size);
    RUN_TEST(test_edge_cases);
    RUN_TEST(test_identical_points);
    
    std::cout << "\nTest suite completed.\n";
    return 0;
}