#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

#include "hpp/Point.hpp"
#include "hpp/medoid.hpp"
#include "hpp/io.hpp"
#include "hpp/vamana_indexing.hpp"
#include "hpp/greedy_search.hpp"

using namespace std;

vector<int> calculateGroundTruthKnns(const vector<Point>& points, const Point& queryPoint, int k) {
    auto query_distance = [&](int pointId) {
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
    vector<int> intersection;
    float recall = 0;
    int numValidFilters = 0;

    for (int i = 0; i < (int)querySet.size(); i++) {
        Point queryPoint = querySet[i];

        int filter = queryPoint.category;

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

void readConfig(const string& filePath, float& alpha, int& L, int& R, int& k, int& L_small, int& R_small, int& R_stitched) {
    ifstream configFile(filePath);
    if (!configFile.is_open()) {
        cerr << "Error: Unable to open configuration file: " << filePath << endl;
        exit(1);
    }

    string line;
    while (getline(configFile, line)) {
        istringstream iss(line);
        string key;
        if (getline(iss, key, ' ')) {
            string value;
            if (getline(iss, value)) {
                if (key == "alpha") alpha = stof(value);
                else if (key == "L") L = stoi(value);
                else if (key == "R") R = stoi(value);
                else if (key == "k") k = stoi(value);
                else if (key == "L_small") L_small = stoi(value);
                else if (key == "R_small") R_small = stoi(value);
                else if (key == "R_stitched") R_stitched = stoi(value);
            }
        }
    }
    configFile.close();
}

int main() {
    const string CONFIG_FILE_PATH = "./config.txt";
    const string DATASET_FILE_PATH = "./input/dummy-data.bin";
    const string QUERY_FILE_PATH = "./input/dummy-queries.bin";

    float alpha = 1.2;
    int L = 80, R = 12, k = 100;
    int L_small = 80, R_small = 12, R_stitched = 12;


    // Read parameters from configuration file
    readConfig(CONFIG_FILE_PATH, alpha, L, R, k, L_small, R_small, R_stitched);

    // Read dataset
    vector<Point> datasetPoints = readDataset(DATASET_FILE_PATH);

    // Read query set
    vector<Point> queryPoints = readQuerySet(QUERY_FILE_PATH);

    set<int> allFilters;
    for (const Point& p : datasetPoints) {
        allFilters.insert(p.category);
    }

    cout << "# Dataset points: " << datasetPoints.size() << endl;
    cout << "# Query points: " << queryPoints.size() << endl;
    cout << "# Dataset filters: " << allFilters.size() << endl;

    // Find medoid mapping
    map<int, int> M = findMedoid(datasetPoints, 1);

    vector<Node> filteredVamanaGraph = filteredVamanaIndexing(datasetPoints, alpha, L, R);
    vector<Node> stitchedVamanaGraph = stitchedVamanaIndexing(datasetPoints, allFilters, alpha, L_small, R_small, R_stitched);

    float filteredVamanaRecall = evaluate(filteredVamanaGraph, datasetPoints, queryPoints, M, allFilters, k, L);
    float stitchedVamanaRecall = evaluate(stitchedVamanaGraph, datasetPoints, queryPoints, M, allFilters, k, L);

    cout << "Filtered Vamana" << endl;
    cout << "k-recall = " << filteredVamanaRecall << " (alpha = " << alpha << ", R = " << R << ", L = " << L << ", k = " << k << ")" << endl;

    cout << "-----------------------------------------------" << endl;

    cout << "Stitched Vamana" << endl;
    cout << "k-recall = " << stitchedVamanaRecall << " (alpha = " << alpha << ", R_small = " << R_small << ", R_stitched = " << R_stitched << ", L_small = " << L_small << ", k = " << k << ")" << endl;

    return 0;
}
