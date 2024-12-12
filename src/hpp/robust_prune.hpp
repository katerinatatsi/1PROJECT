#ifndef ROBUST_PRUNE_HPP
#define ROBUST_PRUNE_HPP

#include <vector>
#include <set>

#include "Point.hpp"
#include "Node.hpp"

using namespace std;

void filteredRobustPrune(vector<Node> &graph, int currentNodeId, set<int> V, float alpha, int R);

void robustPrune(vector<Node> &graph, int currentNodeId, set<int> V, float alpha, int R);

#endif  //ROBUST_PRUNE_HPP
