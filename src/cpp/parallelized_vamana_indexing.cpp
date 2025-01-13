#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <random>
#include <map>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <thread>

#include "../hpp/vamana_indexing.hpp"
#include "../hpp/greedy_search.hpp"
#include "../hpp/robust_prune.hpp"
#include "../hpp/medoid.hpp"

using namespace std;

// Create an empty graph that contains only nodes with no edges
vector<Node> createEmptyGraph(const vector<Point>& points) {
    // cout << "Creating the graph..." << endl;

    // cout << points.size() << endl;

    vector<Node> graph;
    vector<int> neighbors;

    for (int i = 0; i < (int)points.size(); i++) {
        Node node(points[i]);
        graph.push_back(node);
    }

    //cout << graph.size() << endl;

    return graph;
}

vector<int> generateRandomPermutation(int N) {
    // Create a vector with numbers 0 to N-1
    vector<int> numbers(N);
    for (int i = 0; i < N; ++i) {
        numbers[i] = i;
    }

    // Seed and create a random number generator
    random_device rd;  // Seed generator
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    // Shuffle the numbers to get a random permutation
    shuffle(numbers.begin(), numbers.end(), gen);

    return numbers;
}

// Thread function for filtered Vamana
void* filteredVamanaWorker(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    for (int i = data->start_idx; i < data->end_idx; i++) {
        int currentId = (*data->randomPermutation)[i];
        
        // Run greedy search
        pair<vector<int>, set<int>> res = filteredGreedySearch(*data->graph, *data->M, 
            (*data->points)[currentId], 1, data->L);
        set<int> candidates = res.second;

        // Lock before modifying graph
        sem_wait(data->graph_mutex);
        
        // Update current node's neighbors
        filteredRobustPrune(*data->graph, currentId, candidates, data->alpha, data->R);
        
        // Update neighbors' connections
        for (int neighborId : (*data->graph)[currentId].neighbors) {
            (*data->graph)[neighborId].neighbors.push_back(currentId);

            if ((int)(*data->graph)[neighborId].neighbors.size() > data->R) {
                set<int> prunedNeighbors((*data->graph)[neighborId].neighbors.begin(), 
                    (*data->graph)[neighborId].neighbors.end());
                filteredRobustPrune(*data->graph, neighborId, prunedNeighbors, 
                    data->alpha, data->R);
            }
        }
        
        sem_post(data->graph_mutex);
    }
    
    return nullptr;
}

vector<Node> filteredVamanaIndexing(const vector<Point>& points, float alpha, int L, int R) {
    vector<Node> graph = createEmptyGraph(points);
    map<int, int> M = findMedoid(points, 1);
    vector<int> randomPermutation = generateRandomPermutation((int)points.size());
    
    // Initialize semaphore
    sem_t graph_mutex;
    sem_init(&graph_mutex, 0, 1);
    
    // Determine number of threads
    const int num_threads = thread::hardware_concurrency();
    vector<pthread_t> threads(num_threads);
    vector<ThreadData> thread_data(num_threads);
    
    // Calculate work distribution
    int chunk_size = randomPermutation.size() / num_threads;
    
    // Create and launch threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = {
            &graph,
            &points,
            &randomPermutation,
            &M,
            nullptr,
            alpha,
            L,
            R,
            i * chunk_size,
            (i == num_threads - 1) ? (int)randomPermutation.size() : (i + 1) * chunk_size,
            &graph_mutex
        };
        
        pthread_create(&threads[i], nullptr, filteredVamanaWorker, &thread_data[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    // Cleanup
    sem_destroy(&graph_mutex);
    
    return graph;
}

vector<Node> initializeGraph(const vector<Point>& points, int R) {
    // cout << "Initializing graph..." << endl;

    const int N = points.size();
    vector<Node> graph;
    int outDegree[N] = {0};

    // Create a graph with circular format where each node has exactly R outgoing edges
    vector<int> neighbors(R);
    for (int i = 0; i < N; i++) {
        neighbors.clear();
        for (int k = 1; k <= R; k++) {
            int neighbor = (i + k) % N;    // Calculate the neighbor node
            neighbors.push_back(neighbor); // Add the corresponding edge
            outDegree[i]++;                // Update out-degree
        }
        Node node(points[i], neighbors);
        graph.push_back(node);
    }

    return graph;
}

int findMedoidId(const vector<Point>& points) {
    // cout << "Finding medoid point..." << endl;

    double minTotalDistance = numeric_limits<double>::infinity();
    int medoidIndex = -1;

    for (size_t i = 0; i < points.size(); i++) {
        double totalDistance = 0.0;
        for (size_t j = 0; j < points.size(); j++)
            if (i != j) 
                totalDistance += points[i] - points[j];
            
        if (totalDistance < minTotalDistance) {
            minTotalDistance = totalDistance;
            medoidIndex = i;
        }
    }

    return medoidIndex;
}

void* vamanaWorker(void* arg) {
    VamanaThreadData* data = (VamanaThreadData*)arg;
    
    for (int i = data->start_idx; i < data->end_idx; i++) {
        int currentId = (*data->randomPermutation)[i];
        
        pair<vector<int>, set<int>> res = greedySearch(*data->graph, data->medoidId, 
            (*data->points)[currentId], 1, data->L);
        set<int> candidates = res.second;

        sem_wait(data->graph_mutex);
        
        robustPrune(*data->graph, currentId, candidates, data->alpha, data->R);
        
        for (int neighborId : (*data->graph)[currentId].neighbors) {
            unordered_set<int> newNeighbors((*data->graph)[neighborId].neighbors.begin(), 
                (*data->graph)[neighborId].neighbors.end());
            newNeighbors.insert(currentId);

            if (newNeighbors.size() > data->R) {
                set<int> prunedNeighbors(newNeighbors.begin(), newNeighbors.end());
                robustPrune(*data->graph, neighborId, prunedNeighbors, data->alpha, data->R);
            }
            else if (find((*data->graph)[neighborId].neighbors.begin(), 
                (*data->graph)[neighborId].neighbors.end(), currentId) == 
                (*data->graph)[neighborId].neighbors.end()) {
                (*data->graph)[neighborId].neighbors.push_back(currentId);
            }
        }
        
        sem_post(data->graph_mutex);
    }
    
    return nullptr;
}

vector<Node> vamanaIndexing(const vector<Point>& points, float alpha, int L, int R) {
    vector<Node> graph = initializeGraph(points, R);
    //int medoidId = findMedoidId(points);
    random_device rd;  // Seed generator
    mt19937 gen(rd()); // Random number generator
    uniform_int_distribution<> dis(0, points.size() - 1);

    int medoidId = dis(gen);
    vector<int> randomPermutation = generateRandomPermutation(points.size());
    
    sem_t graph_mutex;
    sem_init(&graph_mutex, 0, 1);
    
    const int num_threads = std::thread::hardware_concurrency();
    vector<pthread_t> threads(num_threads);
    vector<VamanaThreadData> thread_data(num_threads);
    
    int chunk_size = randomPermutation.size() / num_threads;
    
    for (int i = 0; i < num_threads; i++) {
        thread_data[i] = {
            &graph,
            &points,
            &randomPermutation,
            medoidId,
            alpha,
            L,
            R,
            i * chunk_size,
            (i == num_threads - 1) ? (int)randomPermutation.size() : (i + 1) * chunk_size,
            &graph_mutex
        };
        
        pthread_create(&threads[i], nullptr, vamanaWorker, &thread_data[i]);
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }
    
    sem_destroy(&graph_mutex);
    
    return graph;
}

void stitchGraphs(vector<Node>& G, const vector<Node>& Gf, map<int, int> M) {
    for (size_t i = 0; i < Gf.size(); i++) {
        auto it = M.find(i);
        if (it == M.end()) {
            continue;
        }
        int global_i = it->second;
        if (global_i < 0 || global_i >= G.size()) {
            continue;
        }
        for (int neighbor : Gf[i].neighbors) {
            auto neighbor_it = M.find(neighbor);
            if (neighbor_it == M.end()) {
                continue;
            }
            int global_neighbor = neighbor_it->second;
            if (global_neighbor < 0 || global_neighbor >= G.size()) {
                continue;
            }
            if (find(G[global_i].neighbors.begin(), G[global_i].neighbors.end(), global_neighbor) == G[global_i].neighbors.end()) {
                G[global_i].neighbors.push_back(global_neighbor);
            }
        }
    }
}

void* stitchedVamanaWorker(void* arg) {
    StitchedThreadData* data = static_cast<StitchedThreadData*>(arg);
    if (!data || !data->main_graph || !data->points) {
        return nullptr;
    }

    try {
        for (int f : data->filter_values) {
            // Create filtered dataset
            vector<Point> Pf;
            map<int, int> local_M;
            
            // Filter points based on category
            for (size_t j = 0; j < data->points->size(); j++) {
                if ((*data->points)[j].category == f) {
                    local_M[Pf.size()] = j;
                    Pf.push_back((*data->points)[j]);
                }
            }

            if (!Pf.empty()) {
                // Build local graph for this filter
                vector<Node> Gf = vamanaIndexing(Pf, data->alpha, data->L, data->R);
                
                // Lock and stitch graphs
                {
                    lock_guard<mutex> lock(*data->graph_mutex);
                    stitchGraphs(*data->main_graph, Gf, local_M);
                }
            }
        }
    } catch (const exception& e) {
        cerr << "Error in stitchedVamanaWorker: " << e.what() << endl;
    }

    return nullptr;
}

vector<Node> stitchedVamanaIndexing(const vector<Point>& points, set<int> filters, float alpha, int L_small, int R_small, int R_stitched) {
    try {
        if (points.empty()) {
            throw runtime_error("Empty points dataset");
        }

        // Validate point dimensions - should be 100 as per the data format
        for (const auto& point : points) {
            if (point.coords.size() != 100) {
                throw runtime_error("Invalid point dimension: " + to_string(point.coords.size()));
            }
        }

        vector<Node> G = createEmptyGraph(points);
        mutex graph_mutex;

        const int max_threads = thread::hardware_concurrency();
        const int num_threads = min(max_threads, static_cast<int>(filters.size()));

        if (num_threads == 0) {
            return G;
        }

        // Distribute filters among threads
        vector<int> filter_vec(filters.begin(), filters.end());
        vector<vector<int>> thread_filters(num_threads);

        for (size_t i = 0; i < filter_vec.size(); i++) {
            thread_filters[i % num_threads].push_back(filter_vec[i]);
        }

        vector<unique_ptr<StitchedThreadData>> thread_data;
        vector<pthread_t> threads(num_threads);

        // Create and launch threads
        for (int i = 0; i < num_threads; i++) {
            thread_data.push_back(make_unique<StitchedThreadData>(
                StitchedThreadData{
                    &G,
                    &points,
                    thread_filters[i],
                    alpha,
                    L_small,
                    R_small,
                    &graph_mutex
                }
            ));

            int rc = pthread_create(&threads[i], nullptr, stitchedVamanaWorker, thread_data[i].get());
            if (rc != 0) {
                throw runtime_error("Failed to create thread " + to_string(i));
            }
        }

        // Wait for threads to complete
        for (int i = 0; i < num_threads; i++) {
            void* status;
            pthread_join(threads[i], &status);
            if (status != nullptr) {
                cerr << "Thread " << i << " failed" << endl;
            }
        }

        // Final pruning step with proper error handling
        for (size_t i = 0; i < points.size(); i++) {
            try {
                set<int> neighborsSet(G[i].neighbors.begin(), G[i].neighbors.end());
                filteredRobustPrune(G, i, neighborsSet, alpha, R_stitched);
            } catch (const exception& e) {
                cerr << "Error pruning node " << i << ": " << e.what() << endl;
                continue;
            }
        }

        return G;

    } catch (const exception& e) {
        cerr << "Error in stitchedVamanaIndexing: " << e.what() << endl;
        return createEmptyGraph(points);
    }
}