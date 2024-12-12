#ifndef IO_HPP
#define IO_HPP

#include <vector>
#include <string>
#include <map>

#include "Point.hpp"

using namespace std;

// Read dataset points
vector<Point> readDataset(const string& filename);

// Read query set points
vector<Point> readQuerySet(const string& filename);

// Save k nearest neighbors in binary format
void saveKnn(const vector<vector<int>> &knns, const string &filename);

#endif // IO_HPP