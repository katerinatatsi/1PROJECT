#include <fstream>
#include <iostream>

#include "../hpp/read_dataset.hpp"

using namespace std;

vector<Point> readFvecs(const string& filename) {
    vector<Point> points;

    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return {};
    }

    while (!file.eof()) {
        int dim;
        
        // Read the dimension (4 bytes = sizeof(int))
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        if (file.eof()) break; // Exit if end of file is reached
        
        Point point;
        point.coords.resize(dim); // Resize the coordinates vector to match dimension

        // Read the vector (dim * sizeof(float) bytes)
        file.read(reinterpret_cast<char*>(point.coords.data()), dim * sizeof(float));
        
        // Add the point to the list of points
        points.push_back(point);
    }

    file.close();

    return points;
}

vector<vector<int>> readIvecs(const string& filename) {
    vector<vector<int>> allGroundTruthNeighbors;

    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return {};
    }

    while (!file.eof()) {
        int dim;
        
        // Read the dimension (4 bytes = sizeof(int))
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        if (file.eof()) break; // Exit if end of file is reached
        
        vector<int> groundTruthNeighbors;
        groundTruthNeighbors.resize(dim); // Resize the coordinates vector to match dimension

        // Read the vector (dim * sizeof(float) bytes)
        file.read(reinterpret_cast<char*>(groundTruthNeighbors.data()), dim * sizeof(int));
        
        // Add the point to the list of points
        allGroundTruthNeighbors.push_back(groundTruthNeighbors);
    }

    file.close();

    return allGroundTruthNeighbors;
}