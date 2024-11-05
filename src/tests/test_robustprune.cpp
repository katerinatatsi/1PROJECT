#include <iostream>
#include <vector>
#include <set>
#include <cassert>              // For using assert statements
#include "../hpp/vamana_indexing.hpp"  // Header για τη συνάρτηση initializeGraph
#include "../hpp/robust_prune.hpp"     // Header για τη συνάρτηση clrobustPrune
#include "../hpp/Point.hpp"
#include "../hpp/Node.hpp"

using namespace std;

void testRobustPrune() {
    // Test 1: Basic pruning test
    vector<Point> points1 = {
        Point({0.0f, 0.0f}),  // Node 0
        Point({1.0f, 0.0f}),  // Node 1
        Point({0.0f, 1.0f}),  // Node 2
        Point({1.0f, 1.0f}),  // Node 3
        Point({2.0f, 0.0f})   // Node 4
    };
    int R1 = 4;
    vector<Node> graph1 = initializeGraph(points1, R1);
    int currentNodeId1 = 0;
    set<int> candidateSet1 = {1, 2, 3, 4};
    float alpha1 = 1.0;
    int maxDegree1 = 2;

    robustPrune(graph1, currentNodeId1, candidateSet1, alpha1, maxDegree1);

    // Assert the number of neighbors is correct after pruning
    assert(graph1[currentNodeId1].neighbors.size() <= maxDegree1);
    cout << "Test 1 passed: Node " << currentNodeId1 << " has " << graph1[currentNodeId1].neighbors.size() << " neighbors after pruning.\n";

    // Test 2: Different alpha and smaller maxDegree
    vector<Point> points2 = {
        Point({0.0f, 0.0f}),  // Node 0
        Point({1.0f, 0.0f}),  // Node 1
        Point({0.0f, 1.0f}),  // Node 2
        Point({1.0f, 1.0f}),  // Node 3
        Point({2.0f, 0.0f})   // Node 4
    };
    int R2 = 4;
    vector<Node> graph2 = initializeGraph(points2, R2);
    int currentNodeId2 = 1;
    set<int> candidateSet2 = {0, 2, 3, 4};
    float alpha2 = 1.3; // Larger distance threshold
    int maxDegree2 = 1; // Smaller max degree

    robustPrune(graph2, currentNodeId2, candidateSet2, alpha2, maxDegree2);

    // Assert the number of neighbors is correct after pruning
    assert(graph2[currentNodeId2].neighbors.size() <= maxDegree2);
    cout << "Test 2 passed: Node " << currentNodeId2 << " has " << graph2[currentNodeId2].neighbors.size() << " neighbors after pruning.\n";

    // Test 3: Larger maxDegree to allow more neighbors
    vector<Point> points3 = {
        Point({0.0f, 0.0f}),  // Node 0
        Point({1.0f, 0.0f}),  // Node 1
        Point({0.0f, 1.0f}),  // Node 2
        Point({1.0f, 1.0f}),  // Node 3
        Point({2.0f, 0.0f}),  // Node 4
        Point({2.0f, 1.0f}),  // Node 5
        Point({3.0f, 0.0f})   // Node 6
    };
    int R3 = 4;
    vector<Node> graph3 = initializeGraph(points3, R3);
    int currentNodeId3 = 2;
    set<int> candidateSet3 = {0, 1, 3, 4, 5, 6};
    float alpha3 = 1.5;
    int maxDegree3 = 3; // Allowing up to 3 neighbors

    robustPrune(graph3, currentNodeId3, candidateSet3, alpha3, maxDegree3);

    // Assert the number of neighbors is correct after pruning
    assert(graph3[currentNodeId3].neighbors.size() <= maxDegree3);
    cout << "Test 3 passed: Node " << currentNodeId3 << " has " << graph3[currentNodeId3].neighbors.size() << " neighbors after pruning.\n";

    cout << "All tests passed successfully." << endl;
}

int main() {
    cout << "Starting robustPrune Tests..." << endl;
    testRobustPrune();
    cout << "Tests completed successfully." << endl;
    return 0;
}