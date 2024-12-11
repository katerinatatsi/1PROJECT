#include "medoid.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <stdexcept>
#include <cstdint>

// Function to find the medoid, taking the dataset filename and the sample size (τ)
std::unordered_map<int, int> FindMedoid(const std::string& dataset_filename, int τ) {
    // Helper function to randomly sample τ elements from the dataset
    auto sample = [](const std::vector<int>& ids, int τ) -> std::vector<int> {
        std::vector<int> sampled_ids;
        if (ids.size() <= τ) {
            return ids; // If the number of ids is less than or equal to τ, return all of them.
        }

        // Use a random number generator for sampling
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, ids.size() - 1);

        std::unordered_set<int> selected_indices;
        while (sampled_ids.size() < τ) {
            int idx = dis(gen);
            if (selected_indices.find(idx) == selected_indices.end()) {
                sampled_ids.push_back(ids[idx]);
                selected_indices.insert(idx);
            }
        }

        return sampled_ids;
    };

    std::ifstream file(dataset_filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        throw std::runtime_error("Error opening file");
    }

    // Read the number of vectors (first 4 bytes as uint32_t)
    uint32_t num_vectors;
    file.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));

    std::cout << "Number of vectors: " << num_vectors << std::endl;

    // If there are no vectors in the file, exit
    if (num_vectors < 1) {
        std::cerr << "No vectors in the file" << std::endl;
        throw std::runtime_error("No vectors in the file");
    }

    // Define the structure for a vector without the T feature
    struct Vector {
        float C;                    // Categorical feature
        float T;                    // Temporal feature
        float values[100];          // 100-dimensional feature vector
    };

    // Create the point list P and the filter set F
    std::vector<std::pair<int, float>> P; // Use float for the categorical feature C
    std::unordered_set<float> F; // Set of unique filters (without rounding)

    // Read the vectors and add the points to the list
    for (uint32_t i = 0; i < num_vectors; ++i) {
        Vector vector;

        // Read the categorical feature C
        file.read(reinterpret_cast<char*>(&vector.C), sizeof(float));

        // Read the temporal feature T
        file.read(reinterpret_cast<char*>(&vector.T), sizeof(float));

        // Read the 100-dimensional feature vector
        file.read(reinterpret_cast<char*>(&vector.values), sizeof(vector.values));

        // Assign the exact categorical feature C to some filter
        float filter = vector.C;

        // Add the point to the list P
        P.push_back({i + 1, filter}); // Use i+1 as the ID for the point

        // Add the filter to the set F
        F.insert(filter);
    }

    // Close the file
    file.close();

    // Count the number of filters
    std::cout << "Number of filters: " << F.size() << std::endl;

    // Print the count of each filter (optional)
    std::unordered_map<float, int> filter_counts;
    for (const auto& pair : P) {
        filter_counts[pair.second]++;
    }

    // Print the distribution of filters (optional)
    // std::cout << "Filter distribution:" << std::endl;
    // for (const auto& entry : filter_counts) {
    //     std::cout << "Filter " << entry.first << ": " << entry.second << " occurrences" << std::endl;
    // }

    // Compute the medoid map
    std::unordered_map<int, int> M;

    // Process each filter
    for (float f : F) {
        // Create Pf: a list of all ids corresponding to the filter f
        std::vector<int> Pf;
        for (const auto& p : P) {
            if (p.second == f) {
                Pf.push_back(p.first);
            }
        }

        // If there are no data for the filter, continue
        if (Pf.empty()) continue;

        // Randomly sample τ points from Pf
        std::vector<int> Rf = sample(Pf, τ);

        // Select the first sample as the medoid
        int p_star = Rf[0];

        // Update the medoid map M
        M[static_cast<int>(f)] = p_star; // Use int for the filter
    }

    return M;
}
