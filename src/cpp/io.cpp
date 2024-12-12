#include <fstream>
#include <iostream>
#include <set>

#include "assert.h"
#include "../hpp/io.hpp"

using namespace std;

vector<Point> readDataset(const string& filename) {
    ifstream file(filename, ios::binary);
    
    if (!file) {
        cerr << "Error opening file" << endl;
        return {};
    }

    // Read the number of vectors (first 4 bytes as int)
    int num_vectors;
    file.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));

    // Check if the file has at least one vector to read
    if (num_vectors < 1) {
        cerr << "No vectors in the file" << endl;
        return {};
    }

    // Create a vector to hold the Point objects
    vector<Point> points;

    for (int i = 0; i < num_vectors; ++i) {
        Point point;

        // Read the categorical attribute C (4 bytes, stored as float and cast to int)
        float categorical_float;
        if (!file.read(reinterpret_cast<char*>(&categorical_float), sizeof(float))) {
            cerr << "Error reading the categorical attribute (C) for vector " << i + 1 << endl;
            return {};
        }
        point.category = static_cast<int>(categorical_float);

        // Skip the T attribute (4 bytes)
        file.seekg(sizeof(float), ios::cur);

        // Read the 100-dimensional vector (100 * 4 bytes)
        vector<float> values(100);
        if (!file.read(reinterpret_cast<char*>(values.data()), values.size() * sizeof(float))) {
            cerr << "Error reading the vector values for vector " << i + 1 << endl;
            return {};
        }
        point.coords = move(values); // Move the vector into the Point object

        // Add the point to the vector
        points.push_back(move(point));
    }

    file.close();

    return points;
}

vector<Point> readQuerySet(const string& filename) {
    ifstream file(filename, ios::binary);
    
    if (!file) {
        cerr << "Error opening file" << endl;
        return {};
    }

    // Read the number of vectors (first 4 bytes as int)
    int num_vectors;
    file.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));

    // Check if the file has at least one vector to read
    if (num_vectors < 1) {
        cerr << "No vectors in the file" << endl;
        return {};
    }

    // Create a vector to hold the filtered Point objects
    vector<Point> points;

    for (int i = 0; i < num_vectors; ++i) {
        float query_type_float, query_value_float;
        
        // Read `query_type` (4 bytes, as float)
        if (!file.read(reinterpret_cast<char*>(&query_type_float), sizeof(float))) {
            cerr << "Error reading query_type for vector " << i + 1 << endl;
            return {};
        }
        int query_type = static_cast<int>(query_type_float);

        // Read `query_value` (4 bytes, as float)
        if (!file.read(reinterpret_cast<char*>(&query_value_float), sizeof(float))) {
            cerr << "Error reading query_value for vector " << i + 1 << endl;
            return {};
        }
        int query_value = static_cast<int>(query_value_float);

        // Skip the next 2 floats (8 bytes)
        file.seekg(2 * sizeof(float), ios::cur);

        // Read the 100-dimensional vector (100 * 4 bytes)
        vector<float> values(100);
        if (!file.read(reinterpret_cast<char*>(values.data()), values.size() * sizeof(float))) {
            cerr << "Error reading the vector values for vector " << i + 1 << endl;
            return {};
        }

        if (query_type == 0 || query_type == 1) {
            Point point;
            point.category = query_value;
            point.coords = move(values);
            points.push_back(move(point));
        }
    }

    file.close();

    return points;
}

void saveKnn(const vector<vector<int>> &knns, const string &filename) {
  ofstream ofs(filename, ios::out | ios::binary);
  const int K = 100;
  const int N = knns.size();

  assert(knns.front().size() == K);

  for (unsigned i = 0; i < N; ++i) {
    auto const &knn = knns[i];
    ofs.write(reinterpret_cast<char const *>(&knn[0]), K * sizeof(int));
  }

  ofs.close();
}