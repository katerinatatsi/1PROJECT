#include "../hpp/acutest.hpp"
#include "../hpp/medoid.hpp"
#include "../hpp/io.hpp"
#include <set>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// Helper function to create a test dataset
void create_test_dataset(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    // Number of vectors
    int num_vectors = 5;
    file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));

    // Sample data points
    struct TestVector {
        float C;  // Category
        float T;  // Additional attribute
        float values[100] = {0};  // 100-dimensional vector
    };

    std::vector<TestVector> vectors = {
        {1.0f, 0.1f},  // First category
        {1.0f, 0.2f},  // First category
        {2.0f, 0.3f},  // Second category
        {2.0f, 0.4f},  // Second category
        {3.0f, 0.5f}   // Third category
    };

    for (auto& vec : vectors) {
        file.write(reinterpret_cast<const char*>(&vec.C), sizeof(vec.C));
        file.write(reinterpret_cast<const char*>(&vec.T), sizeof(vec.T));
        file.write(reinterpret_cast<const char*>(&vec.values), sizeof(vec.values));
    }

    file.close();
}

// Test function for findMedoid
void test_basic_functionality_medoid() {
    const std::string filename = "test_dataset.bin";

    // Create test dataset
    create_test_dataset(filename);

    // Read the points
    std::vector<Point> points = readDataset(filename);

    // Call findMedoid with a threshold
    auto medoids = findMedoid(points, 100);

    // Check the number of medoids
    TEST_CHECK(medoids.size() == 3); // There are 3 unique categories (1, 2, 3)

    // Verify categories
    std::set<int> unique_categories = {1, 2, 3};
    for (const auto& [category, _] : medoids) {
        TEST_CHECK(unique_categories.count(category) > 0);
        unique_categories.erase(category);
    }

    TEST_CHECK(unique_categories.empty()); // Ensure all categories are represented

    // Remove test file
    std::remove(filename.c_str());
}

// Test list
TEST_LIST = {
    {"test_basic_functionality_medoid", test_basic_functionality_medoid},
    {NULL, NULL}
};