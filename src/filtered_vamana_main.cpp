#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/ipc.h>
#include <chrono>

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
    // Δημιουργία ή πρόσβαση στη μνήμη κοινής χρήσης
    key_t key = ftok("/tmp", 'V');
    int shmid = create_vamana_shared_memory(key);
    VamanaSharedMemory* shm = attach_vamana_shared_memory(shmid);

    const string CONFIG_FILE_PATH = "./config.txt";
    const string DATASET_FILE_PATH = "./input/dummy-data.bin";
    const string QUERY_FILE_PATH = "./input/dummy-queries.bin";
    
    // Read configuration parameters
    float alpha;
    int L, R, k, L_small, R_small, R_stitched;
    readConfig(CONFIG_FILE_PATH, alpha, L, R, k, L_small, R_small, R_stitched);
    // Ανάγνωση δεδομένων και ερωτημάτων
    vector<Point> datasetPoints = readDataset(DATASET_FILE_PATH);
    vector<Point> queryPoints = readQuerySet(QUERY_FILE_PATH);

    set<int> allFilters;
    for (const Point& p : datasetPoints) {
        allFilters.insert(p.category);
    }

    // Εμφάνιση πληροφοριών
    cout << "# Dataset points: " << datasetPoints.size() << endl;
    cout << "# Query points: " << queryPoints.size() << endl;
    cout << "# Dataset filters: " << allFilters.size() << endl;

    map<int, int> M = findMedoid(datasetPoints, 1);

    // Δημιουργία του γραφήματος Vamana
    auto start = chrono::high_resolution_clock::now();
    vector<Node> filteredVamanaGraph = filteredVamanaIndexing(datasetPoints, alpha, L, R);
    auto end = chrono::high_resolution_clock::now();
    double filteredTime = chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0;

    // Υπολογισμός recall
    float filteredVamanaRecall = evaluate(filteredVamanaGraph, datasetPoints, queryPoints, M, allFilters, k, L);

    // Αποθήκευση αποτελεσμάτων στη μνήμη κοινής χρήσης
    sem_wait(&shm->results_mutex);
    shm->filtered_vamana_recall = filteredVamanaRecall;
    shm->filtered_complete = true;
    sem_post(&shm->results_mutex);

    // Σήμανση ολοκλήρωσης
    sem_post(&shm->filtered_sem);

    // Εμφάνιση αποτελεσμάτων
    cout << "-----------------------------------------------" << endl;
    cout << "Filtered Vamana Complete" << endl;
    cout << "Build time: " << filteredTime << "s" << endl;
    cout << "k-recall = " << filteredVamanaRecall << " (a = " << alpha << ", R = " << R << ", L = " << L << ", k = " << k << ")" << endl;

    detach_vamana_shared_memory(shm);
    return 0;
}
