#include <iostream>
#include <vector>

#include "hpp/Point.hpp"
#include "hpp/read_fvecs.hpp"
#include "hpp/medoid.hpp"
#include "hpp/graph.hpp"
#include "hpp/random_permutation.hpp"

using namespace std;

int main() {
    // Step 1: Read points coordinates from fvecs file and store them in `points` object
    const string FILENAME = "./input/siftsmall/siftsmall_base.fvecs";

    vector<Point> points = readFvecs(FILENAME);

    // Step 2: Initialize a random R-regular directed graph
    const int N = points.size();
    const int R = 64;

    vector<vector<int>> graph = createGraph(N, R);

    // Step 3: Find the medoid of dataset
    int medoidIndex = findMedoidIndex(points);
    Point medoidPoint = points[medoidIndex];

    // Step 4: Randomly permute the numbers {0, 1, ..., N-1}
    vector<int> randomPermuation = generateRandomPermutation(N);

    return 0;
}
