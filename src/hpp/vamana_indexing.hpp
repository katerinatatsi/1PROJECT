#ifndef VAMANA_INDEXING_HPP
#define VAMANA_INDEXING_HPP

#include <vector>

#include "Point.hpp"
#include "Node.hpp"

using namespace std;

// Create an R-regular directed graph from points database
vector<Node> initializeGraph(const vector<Point>& points, int R);

// Find medoid point using the brute-force technique
int findMedoidId(const vector<Point>& points);

// Generate a random permutation of the numbers {0, 1, ..., N-1}
vector<int> generateRandomPermutation(int N);

// Generate the Vamana graph from points database and with parameters L and R
vector<Node> vamana_indexing(const vector<Point>& points, int L, int R);

#endif // VAMANA_INDEXING_HPP