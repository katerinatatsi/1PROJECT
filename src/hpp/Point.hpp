#ifndef POINT_HPP
#define POINT_HPP

#include <vector>
#include <cmath>

using namespace std;

// Define the Point struct with a dynamic-dimensional coordinates vector
struct Point {
    vector<float> coords;  // Coordinates in a dynamically-sized space
};

// Overloaded - operator to calculate Euclidean distance between two points
float operator-(const Point& a, const Point& b);

#endif // POINT_HPP