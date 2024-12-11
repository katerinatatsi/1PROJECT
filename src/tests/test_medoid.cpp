#include "acutest.h"
#include "medoid.h"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// Helper function to create a test dataset and write it to a binary file
void create_test_dataset(const std::string& filename, const std::vector<std::pair<float, float>>& data) {
    std::ofstream file(filename, std::ios::binary);

    uint32_t num_vectors = data.size();
    file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));

    struct Vector {
        float C;
        float T;
        float values[100] = {0}; // Initialize the remaining dimensions to zero
    };

    // Write each vector's data (C, T, and values) into the file
    for (const auto& [C, T] : data) {
        Vector vec = {C, T};
        file.write(reinterpret_cast<const char*>(&vec.C), sizeof(vec.C));
        file.write(reinterpret_cast<const char*>(&vec.T), sizeof(vec.T));
        file.write(reinterpret_cast<const char*>(&vec.values), sizeof(vec.values));
    }

    file.close();
}

// Test function for the FindMedoid function (basic functionality)
void test_basic_functionality_ignore_τ() {
    const std::string filename = "test_dataset.bin";

    // Create the dataset with some test data
    std::vector<std::pair<float, float>> data = {
        {1.0f, 0.1f}, {1.0f, 0.2f}, {2.0f, 0.3f}, {2.0f, 0.4f}, {3.0f, 0.5f}
    };
    create_test_dataset(filename, data);

    // Call the FindMedoid function
    auto medoids = FindMedoid(filename, 100); // The value of τ doesn't matter here

    // Check that the number of medoids is correct (there should be 3 filters: 1.0, 2.0, 3.0)
    TEST_CHECK(medoids.size() == 3); // There are 3 filters (1.0, 2.0, 3.0)

    // Expected medoids for each filter
    std::unordered_map<int, std::vector<int>> expected_filters = {
        {1, {1, 2}}, {2, {3, 4}}, {3, {5}}
    };

    // Verify that the medoids correspond to one of the IDs for each filter
    for (const auto& [filter, medoid_id] : medoids) {
        TEST_CHECK(expected_filters.count(filter) == 1); // The filter exists
        const auto& ids = expected_filters[filter];
        TEST_CHECK(std::find(ids.begin(), ids.end(), medoid_id) != ids.end());
    }

    // Clean up the test file after the test
    std::remove(filename.c_str());
}

// List of test cases
TEST_LIST = {
    {"test_basic_functionality_ignore_τ", test_basic_functionality_ignore_τ},
    {NULL, NULL}
};
