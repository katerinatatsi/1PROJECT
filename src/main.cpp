#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include "hpp/Point.hpp"
#include "hpp/read_dataset.hpp"
#include "hpp/vamana_indexing.hpp"
#include "hpp/greedy_search.hpp"

using namespace std;

int main() {
    // Read base points coordinates from fvecs file
    const string BASE_FILENAME = "./input/siftsmall/siftsmall_base.fvecs";
    vector<Point> basePoints = readFvecs(BASE_FILENAME);

    // Read query points coordinates from fvecs file
    const string QUERY_FILENAME = "./input/siftsmall/siftsmall_query.fvecs";
    vector<Point> queryPoints = readFvecs(QUERY_FILENAME);

    // Read ground truth neighbor ids from ivecs file
    const string GROUNDTRUTH_FILENAME = "./input/siftsmall/siftsmall_groundtruth.ivecs";
    vector<vector<int>> allGroundtruthNearestNeighbors = readIvecs(GROUNDTRUTH_FILENAME);

    // Run the Vamana indexing algorithm for the specified parameters
    const int L = 200;
    const int R = 20;

    // Keep only the first 1000 points of the database (for testing purposes only)
    // basePoints.resize(1000);

    vector<Node> graph = vamana_indexing(basePoints, L, R);

    // Run the greedy search algorithm to find the approximate nearest neighbors for the query vectors
    int medoidId = findMedoidId(basePoints);
    const int k = 100;

    vector<vector<int>> allApproximateNearestNeighbors;

    for (Point queryPoint: queryPoints) {
        pair<vector<int>, set<int>> res = greedySearch(graph, medoidId, queryPoint, k, L);
        allApproximateNearestNeighbors.push_back(res.first);
    }

    if (allGroundtruthNearestNeighbors.size() != allApproximateNearestNeighbors.size())
        throw invalid_argument("Vectors must have the same size (100)");
    
    // Calculate k@recall
    vector<int> groundtruthNearestNeighbors, approximateNearestNeighbors, interesection;
    float recall = 0;
    for (size_t i = 0; i < allGroundtruthNearestNeighbors.size(); i++) {
        groundtruthNearestNeighbors.assign(allGroundtruthNearestNeighbors[i].begin(), allGroundtruthNearestNeighbors[i].end());
        approximateNearestNeighbors.assign(allApproximateNearestNeighbors[i].begin(), allApproximateNearestNeighbors[i].end());

        sort(groundtruthNearestNeighbors.begin(), groundtruthNearestNeighbors.end());
        sort(approximateNearestNeighbors.begin(), approximateNearestNeighbors.end());

        interesection.clear();
        set_intersection(groundtruthNearestNeighbors.begin(), groundtruthNearestNeighbors.end(), approximateNearestNeighbors.begin(), approximateNearestNeighbors.end(), back_inserter(interesection));

        recall += (float)interesection.size() / (float)approximateNearestNeighbors.size();
    }

    recall /= allGroundtruthNearestNeighbors.size();
    cout << "k-recall = " << recall << " (R = " << R << ", L = " << L << ", k = " << k << ")" << endl;

    return 0;
}