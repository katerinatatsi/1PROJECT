#ifndef MEDOID_HPP
#define MEDOID_HPP

#include <vector>
#include <cmath>

#include "Point.hpp"

using namespace std;

// Print coordinates of medoid point
void printMedoid(const vector<Point>& points, int medoid_index);

// Find medoid point using the brute-force technique
int findMedoidIndex(const vector<Point>& points);

#endif // MEDOID_HPP