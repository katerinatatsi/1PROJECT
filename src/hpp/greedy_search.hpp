#ifndef GREEDY_SEARCH_HPP
#define GREEDY_SEARCH_HPP

#include <vector>
#include <set>
#include <utility>

#include "Point.hpp"
#include "Node.hpp"

using namespace std;

pair<vector<int>, set<int>> greedySearch(
    const vector<Node>& graph,
    int startNodeId, 
    const Point& queryPoint, 
    int k, 
    int L
);

#endif // GREEDY_SEARCH_HPP