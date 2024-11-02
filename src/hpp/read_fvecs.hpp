#ifndef READ_FVECS_HPP
#define READ_FVECS_HPP

#include <vector>
#include <string>

#include "Point.hpp"

using namespace std;

// Read points coordinates from fvecs file
vector<Point> readFvecs(const string& filename);

#endif // READ_FVECS_HPP