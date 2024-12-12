#include "../hpp/Node.hpp"

using namespace std;

// Constructor implementation
Node::Node(Point p) : point(p), neighbors() {}

Node::Node(Point p, vector<int> Neighbors): point(p), neighbors() {}
