#include <iostream>
#include <vector>
#include <set>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include "hpp/Point.hpp"
#include "hpp/medoid.hpp"
#include "hpp/io.hpp"
#include "hpp/vamana_indexing.hpp"
#include "hpp/shared_vamana.hpp"
#include "hpp/evaluate.hpp"

using namespace std;

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
    // Attach to shared memory
    key_t key = ftok("/tmp", 'V');
    int shmid = shmget(key, sizeof(VamanaSharedMemory), 0666);
    if (shmid == -1) {
        cerr << "Failed to get shared memory. Make sure filtered_vamana is running first." << endl;
        return 1;
    }
    VamanaSharedMemory* shm = attach_vamana_shared_memory(shmid);

    const string CONFIG_FILE_PATH = "./config.txt";
    const string DATASET_FILE_PATH = "./input/dummy-data.bin";
    const string QUERY_FILE_PATH = "./input/dummy-queries.bin";
    
    // Read configuration parameters
    float alpha;
    int L, R, k, L_small, R_small, R_stitched;
    readConfig(CONFIG_FILE_PATH, alpha, L, R, k, L_small, R_small, R_stitched);
    
    vector<Point> datasetPoints = readDataset(DATASET_FILE_PATH);
    vector<Point> queryPoints = readQuerySet(QUERY_FILE_PATH);

    set<int> allFilters;
    for (const Point& p: datasetPoints) {
        allFilters.insert(p.category);
    }

    cout << "# Dataset points: " << datasetPoints.size() << endl;
    cout << "# Query points: " << queryPoints.size() << endl;
    cout << "# Dataset filters: " << allFilters.size() << endl;

    map<int, int> M = findMedoid(datasetPoints, 1);

    auto start = chrono::high_resolution_clock::now();
    vector<Node> stitchedVamanaGraph = stitchedVamanaIndexing(datasetPoints, allFilters, alpha, L_small, R_small, R_stitched);
    auto end = chrono::high_resolution_clock::now();
    double stitchedTime = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;

    float stitchedVamanaRecall = evaluate(stitchedVamanaGraph, datasetPoints, queryPoints, M, allFilters, k, L);

    // Store results in shared memory
    sem_wait(&shm->results_mutex);
    shm->stitched_vamana_recall = stitchedVamanaRecall;
    shm->stitched_complete = true;
    sem_post(&shm->results_mutex);
    
    // Signal completion
    sem_post(&shm->stitched_sem);

    cout << "-----------------------------------------------" << endl;
    cout << "Stitched Vamana Complete" << endl;
    cout << "Build time: " << stitchedTime << "s" <<endl;
    cout << "k-recall = " << stitchedVamanaRecall << " (a = " << alpha << ", R_small = " << R_small 
         << ", R_stitched = " << R_stitched << ", L_small = " << L_small << ", k = " << k << ")" << endl;

    detach_vamana_shared_memory(shm);
    return 0;
}