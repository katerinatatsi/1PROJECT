#include <iostream>
#include <cmath>
#include <vector>
#include <utility>
#include <set>

#include "Point.hpp"




using namespace std;

// Define a node in the graph - This must come BEFORE the greedySearch function
struct Node {
    int id;
    Point position;
    std::vector<int> neighbors;
};

float calculateDistance(const Point& p1, const Point& p2);

std::pair<std::vector<int>, std::set<int>> greedySearch(
    const vector<Node>& nodes,
    int start, 
    Point query, 
    int k, 
    int L);





