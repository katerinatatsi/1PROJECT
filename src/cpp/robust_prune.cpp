#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#include "../hpp/robust_prune.hpp"

using namespace std;

void robustPrune(vector<Node> &graph, int currentNodeId, set<int> V, float alpha, int R) {
    set<int> candidateSet(V.begin(), V.end());

    // Update candidateSet
    for (int neighborId : graph[currentNodeId].neighbors)
        candidateSet.insert(neighborId);
    candidateSet.erase(currentNodeId);

    // Clear 
    graph[currentNodeId].neighbors.clear();

    // απόσταση από το current node
    auto query_distance = [&](int nodeId) {
        return graph[nodeId].point - graph[currentNodeId].point;
    };

    // Main pruning loop
    while (!candidateSet.empty()) {
        auto bestNodeIt = min_element(
            candidateSet.begin(), 
            candidateSet.end(),
            [&](int a, int b) { return query_distance(a) < query_distance(b); }
        );
        int bestNodeId = *bestNodeIt;

        graph[currentNodeId].neighbors.push_back(bestNodeId);

        if (graph[currentNodeId].neighbors.size() == R)
            break;
        
        // prune ανάλογα το  distance threshlod alpha 
        vector<int> nodesToRemove;
        for (int nodeId: candidateSet)
            if (alpha * (graph[bestNodeId].point - graph[nodeId].point) <= graph[currentNodeId].point - graph[nodeId].point)
                nodesToRemove.push_back(nodeId);
        
        // Remove από το candidateSet όσα "φάγανε" prune
        for (int nodeId : nodesToRemove)
            candidateSet.erase(nodeId);
    }
}