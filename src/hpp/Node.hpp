#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <vector>
#include <cmath>

#include "Point.hpp"

using namespace std;

// Define a node in the graph
struct Node {
    Point point;
    vector<int> neighbors;

    // Constructor
    Node(Point p);

    Node(Point p, vector<int> Neighbors);
};

#endif // NODE_HPP
