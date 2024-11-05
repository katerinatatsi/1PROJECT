#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cstring> 
#include <vector>
#include <cmath> 
#include "hpp/greedy_search.hpp"
#include "hpp/Point.hpp"
#include "hpp/Node.hpp"

using namespace std;

// Constants for test setup
const int N = 10;  // Number of nodes
const int D = 3;   // Dimension of points

// Helper functions
void setPointCoordinates(Point& point, float value) {
    point.coords.resize(D, value);
}

vector<Node> createTestGraph(int N) {
    vector<Node> nodes;
    nodes.reserve(N);  // Reserve space to avoid reallocations
    
    // Create N nodes
    for (int i = 0; i < N; i++) {
        // Create point for current node
        Point p;
        setPointCoordinates(p, static_cast<float>(i));
        
        // Create neighbors list (circular graph)
        vector<int> neighbors;
        neighbors.reserve(4);  // We'll add 4 neighbors per node
        for (int k = 1; k <= 2; k++) {
            neighbors.push_back((i + k) % N);
            neighbors.push_back((i - k + N) % N);
        }
        
        // Create and add the node
        nodes.emplace_back(std::move(p), std::move(neighbors));
    }
    
    return nodes;
}

// Test framework macros
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

// Test cases
TEST_CASE(test_finds_starting_node) {
    vector<Node> nodes = createTestGraph(N);
    
    Point query;
    setPointCoordinates(query, 0.0f);

    auto [result, visited] = greedySearch(nodes, 0, query, 1, 2);
    
    ASSERT_TRUE(!result.empty());
    ASSERT_EQ(result[0], 0);
}

TEST_CASE(test_finds_k_nearest) {
    vector<Node> nodes = createTestGraph(N);
    
    Point query;
    setPointCoordinates(query, 0.0f);

    auto [result, visited] = greedySearch(nodes, 0, query, 3, 4);
    
    ASSERT_EQ(result.size(), 3);
    
    // Check if results are sorted by distance
    for (size_t i = 1; i < result.size(); i++) {
        ASSERT_TRUE(
            (nodes[result[i-1]].point - query) <=
            (nodes[result[i]].point - query)
        );
    }
}

TEST_CASE(test_respects_list_size) {
    vector<Node> nodes = createTestGraph(N);
    
    Point query;
    setPointCoordinates(query, 0.0f);

    auto [result, visited] = greedySearch(nodes, 0, query, N, 2);
    
    ASSERT_LE(visited.size(), static_cast<size_t>(N));
    ASSERT_LE(result.size(), static_cast<size_t>(N));
}

TEST_CASE(test_edge_cases) {
    vector<Node> nodes = createTestGraph(N);
    
    Point query;
    setPointCoordinates(query, 0.0f);
    
    auto [result1, visited1] = greedySearch(nodes, 0, query, 0, 2);
    ASSERT_TRUE(result1.empty());

    auto [result2, visited2] = greedySearch(nodes, 0, query, 2, 1);
    ASSERT_LE(visited2.size(), static_cast<size_t>(N));
}


TEST_CASE(test_identical_points) {
    // Create nodes with identical points
    vector<Node> nodes;
    nodes.reserve(N);
    
    Point p;
    setPointCoordinates(p, 1.0f);
    
    for (int i = 0; i < N; i++) {
        vector<int> neighbors;
        neighbors.reserve(4);
        for (int k = 1; k <= 2; k++) {
            neighbors.push_back((i + k) % N);
            neighbors.push_back((i - k + N) % N);
        }
        nodes.emplace_back(p, std::move(neighbors));
    }
    
    Point query;
    setPointCoordinates(query, 1.0f);

    auto [result, visited] = greedySearch(nodes, 0, query, 3, 3);
    
    ASSERT_EQ(result.size(), 3);
    
    for (size_t i = 1; i < result.size(); i++) {
        float dist1 = nodes[result[0]].point - query;
        float dist2 = nodes[result[i]].point - query;
        ASSERT_TRUE(abs(dist1 - dist2) < 1e-6f);
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
