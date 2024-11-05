#ifndef POINT_HPP
#define POINT_HPP

#include <vector>
#include <cmath>

using namespace std;

struct Point {
    vector<float> coords;  // Coordinates in a dynamically-sized space

    // Constructor που δέχεται λίστα τιμών για αρχικοποίηση των συντεταγμένων
    Point(const std::initializer_list<float>& init_coords) : coords(init_coords) {}

    // Προαιρετικά μπορούμε να έχουμε και έναν default constructor
    Point() = default;

    // Άλλοι constructors και συναρτήσεις αν χρειάζεται

};

// Overloaded - operator to calculate Euclidean distance between two points
float operator-(const Point& a, const Point& b);

#endif // POINT_HPP