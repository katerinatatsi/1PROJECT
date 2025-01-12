#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include "Point.hpp"
#include "Node.hpp"
#include <map>
#include <set>

using namespace std;

vector<int> calculateGroundTruthKnns(const vector<Point>& points, const Point& queryPoint, int k);

float evaluate(
    const vector<Node> graph, 
    const vector<Point>& dataset, 
    const vector<Point>& querySet, 
    map<int, int> M, 
    set<int> filters,
    int k,
    int L
);

#endif
