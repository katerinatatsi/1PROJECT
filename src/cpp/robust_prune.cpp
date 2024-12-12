#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#include "../hpp/robust_prune.hpp"

using namespace std;

void filteredRobustPrune(vector<Node> &graph, int currentNodeId, set<int> V, float alpha, int R) {
    // cout << "Running Filtered Robust Prune..." << endl;

    set<int> candidateSet(V.begin(), V.end());

    // Update candidateSet
    for (int neighborId : graph[currentNodeId].neighbors)
        candidateSet.insert(neighborId);
    candidateSet.erase(currentNodeId);

    // Clear the current neighbors list for new assignment
    graph[currentNodeId].neighbors.clear();

    // Lambda function to calculate distance from the current node
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

        if ((int)graph[currentNodeId].neighbors.size() == R)
            break;
        
        // Prepare to prune nodes based on the distance threshold
        vector<int> nodesToRemove;
        for (int nodeId: candidateSet) {
            if (graph[currentNodeId].point.category != graph[nodeId].point.category || graph[nodeId].point.category != graph[bestNodeId].point.category)
                continue;
            
            if (alpha * (graph[bestNodeId].point - graph[nodeId].point) <= graph[currentNodeId].point - graph[nodeId].point)
                nodesToRemove.push_back(nodeId);
        }
        
        // Remove pruned nodes from the candidate set
        for (int nodeId : nodesToRemove)
            candidateSet.erase(nodeId);
    }
}

void robustPrune(vector<Node> &graph, int currentNodeId, set<int> V, float alpha, int R) {
    // cout << "Performing robust prune..." << endl;

    set<int> candidateSet(V.begin(), V.end());

    // Update candidateSet
    for (int neighborId : graph[currentNodeId].neighbors)
        candidateSet.insert(neighborId);
    candidateSet.erase(currentNodeId);

    // Clear the current neighbors list for new assignment
    graph[currentNodeId].neighbors.clear();

    // Lambda function to calculate distance from the current node
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
        
        // Prepare to prune nodes based on the distance threshold
        vector<int> nodesToRemove;
        for (int nodeId: candidateSet)
            if (alpha * (graph[bestNodeId].point - graph[nodeId].point) <= graph[currentNodeId].point - graph[nodeId].point)
                nodesToRemove.push_back(nodeId);
        
        // Remove pruned nodes from the candidate set
        for (int nodeId : nodesToRemove)
            candidateSet.erase(nodeId);
    }
} 
