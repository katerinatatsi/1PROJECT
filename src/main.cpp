#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <map>

#include "hpp/Point.hpp"
#include "hpp/medoid.hpp"
#include "hpp/io.hpp"
#include "hpp/vamana_indexing.hpp"
#include "hpp/greedy_search.hpp"

using namespace std;

vector<int> calculateGroundTruthKnns(const vector<Point>& points, const Point& queryPoint, int k) {
    auto query_distance = [&](int pointId) {
        // cout << nodeId << endl;
        return points[pointId] - queryPoint;
    };

    vector<int> pointIdsToConsider;

    for (int i = 0; i < (int)points.size(); i++)
        if (queryPoint.category == -1 || (queryPoint.category >= 0 && points[i].category == queryPoint.category))
            pointIdsToConsider.push_back(i);

    sort(pointIdsToConsider.begin(), pointIdsToConsider.end(), [&](int a, int b) { return query_distance(a) < query_distance(b); });

    if ((int)pointIdsToConsider.size() > k)
        pointIdsToConsider.resize(k);

    return pointIdsToConsider;
}

float evaluate(
    const vector<Node> graph, 
    const vector<Point>& dataset, 
    const vector<Point>& querySet, 
    map<int, int> M, 
    set<int> filters,
    int k,
    int L
) {
    // Run the greedy search algorithm to find the approximate nearest neighbors for the query vectors
    vector<int> intersection;
    float recall = 0;
    int numValidFilters = 0;

    for (int i = 0; i < (int)querySet.size(); i++) {
        Point queryPoint = querySet[i];

        int filter = queryPoint.category;

        // Make sure that filter is valid
        if (filters.find(filter) != filters.end()) {
            numValidFilters++;

            pair<vector<int>, set<int>> res = filteredGreedySearch(graph, M, queryPoint, k, L);
            vector<int> approximateNearestNeighbors = res.first;
            vector<int> groundTruthNeighbors = calculateGroundTruthKnns(dataset, queryPoint, k);

            sort(groundTruthNeighbors.begin(), groundTruthNeighbors.end());
            sort(approximateNearestNeighbors.begin(), approximateNearestNeighbors.end());

            intersection.clear();
            set_intersection(groundTruthNeighbors.begin(), groundTruthNeighbors.end(), approximateNearestNeighbors.begin(), approximateNearestNeighbors.end(), back_inserter(intersection));

            recall += (float)intersection.size() / (float)k;
        }
    }

    cout << "# valid filters = " << numValidFilters << endl;

    recall /= numValidFilters;

    return recall;
}

int main() {
    const string DATASET_FILE_PATH = "./input/dummy-data.bin";
    const string QUERY_FILE_PATH = "./input/dummy-queries.bin";
    
    // Read dataset
    vector<Point> datasetPoints = readDataset(DATASET_FILE_PATH);

    // Read query set
    vector<Point> queryPoints = readQuerySet(QUERY_FILE_PATH);

    set<int> allFilters;
    for (const Point& p: datasetPoints) {
        // Append all filters to their separate set
        allFilters.insert(p.category);
    }

    cout << "# Dataset points: " << datasetPoints.size() << endl;
    cout << "# Query points: " << queryPoints.size() << endl;
    cout << "# Dataset filters: " << allFilters.size() << endl;

    // Find medoid mapping
    map<int, int> M = findMedoid(datasetPoints, 1);

    // Run the Vamana indexing algorithm for the specified parameters
    const float alpha = 1.2;
    const int L = 80;
    const int R = 12;

    const int L_small = 80;
    const int R_small = 12;
    const int R_stitched = 12;

    // Keep only the first 1000 points of the dataset (for testing purposes only)
    // datasetPoints.resize(10);

    vector<Node> filteredVamanaGraph = filteredVamanaIndexing(datasetPoints, alpha, L, R);
    vector<Node> stitchedVamanaGraph = stitchedVamanaIndexing(datasetPoints, allFilters, alpha, L_small, R_small, R_stitched);

    // Run the greedy search algorithm to find the approximate nearest neighbors for the query vectors
    const int k = 100;

    float filteredVamanaRecall = evaluate(filteredVamanaGraph, datasetPoints, queryPoints, M, allFilters, k, L);
    float stitchedVamanaRecall = evaluate(stitchedVamanaGraph, datasetPoints, queryPoints, M, allFilters, k, L);

    cout << "Filtered Vamana" << endl;
    cout << "k-recall = " << filteredVamanaRecall << " (a = " << alpha << ", R = " << R << ", L = " << L << ", k = " << k << ")" << endl;

    cout << "-----------------------------------------------" << endl;

    cout << "Stitched Vamana" << endl;
    cout << "k-recall = " << stitchedVamanaRecall << " (a = " << alpha << ", R_small = " << R_small << ", R_stitched =  " << R_stitched << ", L_small = " << L_small << ", k = " << k << ")" << endl;
    
    return 0;
}
