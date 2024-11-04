#ifndef READ_DATASET_HPP
#define READ_DATASET_HPP

#include <vector>
#include <string>

#include "Point.hpp"

using namespace std;

// Read points coordinates from fvecs file
vector<Point> readFvecs(const string& filename);

// Read points coordinates from ivecs file
vector<vector<int>> readIvecs(const string& filename);

#endif // READ_DATASET_HPP