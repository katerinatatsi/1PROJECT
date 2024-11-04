#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>

#include "../hpp/greedy_search.hpp"

using namespace std;

pair<vector<int>, set<int>> greedySearch(const vector<Node>& graph, int startNodeId, const Point& queryPoint, int k, int L) {
    // cout << "Performing greedy search..." << endl;
    
    set<int> unexploredNodes = {startNodeId};
    set<int> visitedNodes;

    auto query_distance = [&](int nodeId) {
        return graph[nodeId].point - queryPoint;
    };
    
    while (true) {
        // Create a vector to store the set difference result
        vector<int> difference;

        // Compute set difference unexploredNodes - visitedNodes
        set_difference(unexploredNodes.begin(), unexploredNodes.end(), visitedNodes.begin(), visitedNodes.end(), back_inserter(difference));

        if (difference.empty())
            break;
        
        // Find the closest node in the difference set
        auto bestNodeIt = min_element(
            difference.begin(), 
            difference.end(),
            [&](int a, int b) { return query_distance(a) < query_distance(b); }
        );
        int bestNodeId = *bestNodeIt;

        // Update visitedNodes set
        visitedNodes.insert(bestNodeId);

        // Update unexploredNodes set with neighbors of bestNodeId
        for (int neighborId : graph[bestNodeId].neighbors)
            unexploredNodes.insert(neighborId);
        
        // Retain only the closest L elements in unexploredNodes
        vector<int> unexploredNodesVec;
        if (unexploredNodes.size() > L) {
            // Avoid reallocation by using `assign`
            unexploredNodesVec.assign(unexploredNodes.begin(), unexploredNodes.end());

            // Use nth_element to get the top L closest nodes
            nth_element(
                unexploredNodesVec.begin(), 
                unexploredNodesVec.begin() + L - 1, 
                unexploredNodesVec.end(),
                [&](int a, int b) { return query_distance(a) < query_distance(b); }
            );

            // Resize and update unexploredNodes with only the closest L elements
            unexploredNodesVec.resize(L);
            unexploredNodes.clear();
            unexploredNodes.insert(unexploredNodesVec.begin(), unexploredNodesVec.end());
        }
    }

    // Sort the remaining unexploredNodes to get the k closest nodes
    vector<int> kNearestNeighbors(unexploredNodes.begin(), unexploredNodes.end());
    sort(
        kNearestNeighbors.begin(), 
        kNearestNeighbors.end(), 
        [&](int a, int b) { return query_distance(a) < query_distance(b); }
    );

    // Retain only the top k closest elements
    if (kNearestNeighbors.size() > k)
        kNearestNeighbors.resize(k);

    return {kNearestNeighbors, visitedNodes};
}