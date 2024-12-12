#ifndef VAMANA_INDEXING_HPP
#define VAMANA_INDEXING_HPP

#include <vector>

#include "Point.hpp"
#include "Node.hpp"

using namespace std;

// Create an empty graph that contains only nodes with no edges
vector<Node> createEmptyGraph(const vector<Point>& points);

// Generate a random permutation of the numbers {0, 1, ..., N-1}
vector<int> generateRandomPermutation(int N);

// Generate the Vamana graph from filtered points database and with parameters L and R
vector<Node> filteredVamanaIndexing(const vector<Point>& points, float alpha, int L, int R);

vector<Node> initializeGraph(const vector<Point>& points, int R);

int findMedoidId(const vector<Point>& points);

vector<Node> vamanaIndexing(const vector<Point>& points, float alpha, int L, int R);

void stitchGraphs(vector<Node>& G, const vector<Node>& Gf, map<int, int> M);

// Generate the stitched Vamana graph from filtered points database and with parameters L and R
vector<Node> stitchedVamanaIndexing(const vector<Point>& points, set<int> filters, float alpha, int L_small, int R_small, int R_stitched);

#endif // VAMANA_INDEXING_HPP
