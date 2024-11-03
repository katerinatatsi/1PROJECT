#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <cmath>

#include "Point.hpp"

using namespace std;

// Define a node in the graph
struct Node {
    Point point;
    vector<int> neighbors;

    Node(Point Point, vector<int> Neighbors);
};

// Overloaded - operator to calculate Euclidean distance between two graph nodes
float operator-(const Node& a, const Node& b);

#endif // NODE_HPP