#include <iostream>
#include <vector>
#include <cassert>               // For using assert statements
#include "../hpp/vamana_indexing.hpp"
#include "../hpp/Point.hpp"

using namespace std;

// Test case to verify that the medoid is correctly calculated for a simple set of points
void testMedoidCalculationSimple() {
    vector<Point> points = {
        Point({20.0f, 20.0f}),
        Point({20.0f, 40.0f}),
        Point({20.0f, 60.0f}),
        Point({35.0f, 60.0f}),
        Point({35.0f, 40.0f}),  // Expected medoid
        Point({35.0f, 20.0f}),
        Point({45.0f, 60.0f}),
        Point({45.0f, 40.0f}),
        Point({45.0f, 20.0f})
    };

    int medoidIndex = findMedoidId(points);
    assert(medoidIndex == 4);  // Expected medoid is at index 4
    cout << "Medoid Test 1 PASSED. Medoid found at index " << medoidIndex
         << " with coordinates (" << points[medoidIndex].coords[0]
         << ", " << points[medoidIndex].coords[1] << ")" << endl;
}

// Additional test case to check medoid calculation with a different set of points
void testMedoidCalculationAdditional() {
    vector<Point> points = {
        Point({6.0f, 25.0f}),
        Point({6.0f, 45.0f}),
        Point({6.0f, 65.0f}),
        Point({4.0f, 25.0f}),
        Point({4.0f, 45.0f}),   // Expected medoid
        Point({4.0f, 65.0f}),
        Point({2.0f, 25.0f}),
        Point({2.0f, 45.0f}),
        Point({2.0f, 65.0f})
    };

    int medoidIndex = findMedoidId(points);
    assert(medoidIndex == 4);  // Expected medoid is at index 4
    cout << "Medoid Test 2 PASSED. Medoid found at index " << medoidIndex
         << " with coordinates (" << points[medoidIndex].coords[0]
         << ", " << points[medoidIndex].coords[1] << ")" << endl;
}

// Third test case to ensure that medoid calculation works for another point set
void testMedoidCalculationThird() {
    vector<Point> points = {
        Point({56.0f, 55.0f}),
        Point({56.0f, 75.0f}),
        Point({56.0f, 95.0f}),
        Point({54.0f, 55.0f}),
        Point({54.0f, 75.0f}),   // Expected medoid
        Point({54.0f, 95.0f}),
        Point({52.0f, 55.0f}),
        Point({52.0f, 75.0f}),
        Point({52.0f, 95.0f})
    };

    int medoidIndex = findMedoidId(points);
    assert(medoidIndex == 4);  // Expected medoid is at index 4
    cout << "Medoid Test 3 PASSED. Medoid found at index " << medoidIndex
         << " with coordinates (" << points[medoidIndex].coords[0]
         << ", " << points[medoidIndex].coords[1] << ")" << endl;
}

// Main function to run all test cases for medoid calculation
int main() {
    cout << "Starting Medoid Tests..." << endl;
    cout << "-----------------------------" << endl;

    testMedoidCalculationSimple();
    testMedoidCalculationAdditional();
    testMedoidCalculationThird();

    cout << "-----------------------------" << endl;
    cout << "All tests completed." << endl;

    return 0;
}
