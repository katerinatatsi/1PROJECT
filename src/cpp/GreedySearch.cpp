#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

#include "../hpp/GreedySearch.hpp"


using namespace std;

float calculateDistance(const Point& p1, const Point& p2) {
    float sum = 0.0f;
    for (size_t i = 0; i < p1.coords.size(); i++) {
        float diff = p1.coords[i] - p2.coords[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}


pair<vector<int>, set<int>> greedySearch(
    const vector<Node>& nodes,
    int start, 
    Point query, 
    int k, 
    int L) 
{
   
    set<int> visited;
    vector<int> candidateList = {start};

  
    auto query_distance = [&](int nodeId) {
        return calculateDistance(nodes[nodeId].position, query);
    };

    
    while (!candidateList.empty()) {
       
        auto bestNodeIt = min_element(
            candidateList.begin(), 
            candidateList.end(),
            [&](int a, int b) { return query_distance(a) < query_distance(b); }
        );
        int bestNode = *bestNodeIt;

     
        visited.insert(bestNode);
        candidateList.erase(bestNodeIt);

      
      for (int neighbor : nodes[bestNode].neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                candidateList.push_back(neighbor);
            }
        }

        if (candidateList.size() > L) {
                nth_element(
                candidateList.begin(), 
                candidateList.begin() + L, 
                candidateList.end(),
                [&](int a, int b) { return query_distance(a) < query_distance(b); }
            );
            candidateList.resize(L);
        }
    }

  
    vector<int> result(visited.begin(), visited.end());
    sort(result.begin(), result.end(), 
        [&](int a, int b) { return query_distance(a) < query_distance(b); }
    );

   
    if (result.size() > k) {
        result.resize(k);
    }
    return {result, visited};
}