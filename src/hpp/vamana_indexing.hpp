#ifndef VAMANA_INDEXING_HPP
#define VAMANA_INDEXING_HPP

#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include "Point.hpp"
#include "Node.hpp"

// Structure to pass data to threads for filtered and stitched Vamana
struct ThreadData {
    std::vector<Node>* graph;
    const std::vector<Point>* points;
    std::vector<int>* randomPermutation;
    std::map<int, int>* M;
    const std::set<int>* filters;
    float alpha;
    int L;
    int R;
    int start_idx;
    int end_idx;
    sem_t* graph_mutex;
};

// Structure for Vamana indexing threads
struct VamanaThreadData {
    std::vector<Node>* graph;
    const std::vector<Point>* points;
    std::vector<int>* randomPermutation;
    int medoidId;
    float alpha;
    int L;
    int R;
    int start_idx;
    int end_idx;
    sem_t* graph_mutex;
};

struct StitchedThreadData {
    vector<Node>* main_graph;
    const vector<Point>* points;
    vector<int> filter_values;
    float alpha;
    int L;
    int R;
    mutex* graph_mutex;
};

// Create an empty graph that contains only nodes with no edges
vector<Node> createEmptyGraph(const vector<Point>& points);

// Generate a random permutation of the numbers {0, 1, ..., N-1}
vector<int> generateRandomPermutation(int N);

// Generate the Vamana graph from filtered points database and with parameters L and R
vector<Node> filteredVamanaIndexing(const vector<Point>& points, float alpha, int L, int R);

vector<Node> initializeGraph(const vector<Point>& points, int R);

int findMedoidId(const vector<Point>& points);

vector<Node> vamanaIndexing(const vector<Point>& points, float alpha, int L, int R);

void stitchGraphs(vector<Node>& G, const vector<Node>& Gf, map<int, int> M);

// Generate the stitched Vamana graph from filtered points database and with parameters L and R
vector<Node> stitchedVamanaIndexing(const vector<Point>& points, set<int> filters, float alpha, int L_small, int R_small, int R_stitched);

// Thread function for filtered Vamana
void* filteredVamanaWorker(void* arg);

// Thread function for stitched Vamana
void* stitchedVamanaWorker(void* arg);

void* vamanaWorker(void* arg);

#endif // VAMANA_INDEXING_HPP
