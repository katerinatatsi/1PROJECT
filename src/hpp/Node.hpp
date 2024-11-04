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

    Node(Point p, vector<int> Neighbors);
};

#endif // NODE_HPP