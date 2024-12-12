#ifndef MEDOID_HPP
#define MEDOID_HPP

#include <vector>
#include <string>
#include <map>

#include "Point.hpp"

using namespace std;

// Function FindMedoid that takes the dataset points and the number of samples (thres)
map<int, int> findMedoid(const vector<Point>& points, int thres);

#endif // MEDOID_HPP
