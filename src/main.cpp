#include <iostream>
#include <vector>

#include "hpp/Point.hpp"
#include "hpp/read_fvecs.hpp"
#include "hpp/vamana_indexing.hpp"

using namespace std;

int main() {
    // Read points coordinates from fvecs file and store them in `points` object
    const string FILENAME = "./input/siftsmall/siftsmall_base.fvecs";
    vector<Point> points = readFvecs(FILENAME);

    // Run the Vamana indexing algorithm for the specified parameters
    const int L = 100;
    const int R = 20;

    vector<Node> graph = vamana_indexing(points, L, R);

    // Run the greedy search algorithm for the query vectors
    // greedySearch(graph, medoidId, queryPoint, k, L)
    
    return 0;
}
