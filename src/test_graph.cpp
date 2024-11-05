#include <iostream>
#include <vector>
#include <cassert>               // For using assert statements
#include "../hpp/vamana_indexing.hpp"  // Header file for initializeGraph
#include "../hpp/Point.hpp"
#include "../hpp/Node.hpp"       // Header file defining the Node structure

using namespace std;

// Test function to verify if the graph is R-regular
void testInitializeGraphRRegular(size_t R, const std::vector<Point>& points) {
    // Initialize the graph based on the provided points and R
    std::vector<Node> graph = initializeGraph(points, R);

    // Flag to track if the graph is R-regular
    bool isRRegular = true;
    for (const auto& node : graph) {
        // Check if each node has exactly R neighbors
        assert(node.neighbors.size() == R && "Node does not have the expected R neighbors.");
    }

    // Output result of the test
    if (isRRegular) {
        cout << "Test PASSED. The graph is " << R << "-regular." << endl;
    } else {
        cout << "Test FAILED. The graph is not " << R << "-regular." << endl;
    }
}

int main() {
    cout << "Starting R-Regular Graph Tests..." << endl;

    // Test for R = 2
    cout << "Testing for R = 2" << endl;
    vector<Point> pointsR2 = {
        Point({0.0f, 0.0f}),
        Point({1.0f, 0.0f}),
        Point({0.0f, 1.0f}),
        Point({1.0f, 1.0f}),
        Point({2.0f, 0.0f}),
        Point({2.0f, 1.0f})
    };
    testInitializeGraphRRegular(2, pointsR2);

    // Test for R = 4 with more points
    cout << "Testing for R = 4" << endl;
    vector<Point> pointsR4 = {
        Point({0.0f, 0.0f}),
        Point({1.0f, 0.0f}),
        Point({0.0f, 1.0f}),
        Point({1.0f, 1.0f}),
        Point({2.0f, 0.0f}),
        Point({2.0f, 1.0f}),
        Point({3.0f, 0.0f}),
        Point({3.0f, 1.0f})
    };
    testInitializeGraphRRegular(4, pointsR4);

    // Test for R = 10 with even more points
    cout << "Testing for R = 10" << endl;
    vector<Point> pointsR10 = {
        Point({0.0f, 0.0f}), Point({1.0f, 0.0f}), Point({2.0f, 0.0f}),
        Point({3.0f, 0.0f}), Point({4.0f, 0.0f}), Point({5.0f, 0.0f}),
        Point({0.0f, 1.0f}), Point({1.0f, 1.0f}), Point({2.0f, 1.0f}),
        Point({3.0f, 1.0f}), Point({4.0f, 1.0f}), Point({5.0f, 1.0f}),
        Point({0.0f, 2.0f}), Point({1.0f, 2.0f}), Point({2.0f, 2.0f}),
        Point({3.0f, 2.0f}), Point({4.0f, 2.0f}), Point({5.0f, 2.0f})
    };
    testInitializeGraphRRegular(10, pointsR10);

    cout << "All tests completed!." << endl;
    return 0;
}
