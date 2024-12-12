#include <iostream>
#include <cmath>
#include <stdexcept>

#include "../hpp/Point.hpp"

using namespace std;

// Non-member overloaded - operator for Euclidean distance
float operator-(const Point& a, const Point& b) {
    // cout << a.coords.size() << endl;
    // cout << b.coords.size() << endl;

    if (a.coords.size() != b.coords.size())
        throw invalid_argument("Points must have the same dimension for Euclidean distance calculation.");

    float sum = 0.0f;
    for (size_t i = 0; i < a.coords.size(); ++i) {
        float diff = a.coords[i] - b.coords[i];
        sum += diff * diff;
    }
    
    return sqrt(sum);
}