#include <iostream>
#include <cmath>
#include <stdexcept>

#include "../hpp/Point.hpp"

using namespace std;

float operator-(const Point& a, const Point& b) {
    if (a.coords.size() != b.coords.size()) {
        throw invalid_argument("Points must have the same dimension: " + 
            to_string(a.coords.size()) + " vs " + 
            to_string(b.coords.size()));
    }
    
    float sum = 0.0f;
    for (size_t i = 0; i < a.coords.size(); i++) {
        float diff = a.coords[i] - b.coords[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}