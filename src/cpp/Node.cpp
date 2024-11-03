#include <cmath>
#include <stdexcept>

#include "../hpp/Node.hpp"
#include "../hpp/Point.hpp"

using namespace std;

Node::Node(Point p, vector<int> Neighbors) {
    point = p;
    neighbors = Neighbors;
}

// Non-member overloaded - operator for Euclidean distance
float operator-(const Node& a, const Node& b) {
    return a.point - b.point;
}
