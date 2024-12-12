#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <random>
#include <iomanip>
#include <cstdint>
#include <stdexcept>
#include <set>
#include <map>

#include "../hpp/medoid.hpp"
#include "../hpp/io.hpp"

using namespace std;

map<int, int> findMedoid(const vector<Point>& points, int thres) {
    // Helper function to randomly sample data
    auto sample = [](const vector<int>& ids, int thres) -> vector<int> {
        vector<int> sampled_ids;
        if ((int)ids.size() <= thres) {
            return ids; // If the ids are less than or equal to thres, return all.
        }

        // Use a random number generator for sampling
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, ids.size() - 1);

        unordered_set<int> selected_indices;
        while ((int)sampled_ids.size() < thres) {
            int idx = dis(gen);
            if (selected_indices.find(idx) == selected_indices.end()) {
                sampled_ids.push_back(ids[idx]);
                selected_indices.insert(idx);
            }
        }

        return sampled_ids;
    };

    set<int> filters;

    for (Point p: points) {
        // Append all filters to their separate set
        filters.insert(p.category);
    }

    // Compute the medoid map
    map<int, int> M;

    // Process each filter
    for (int f : filters) {
        // Create Pf: list with all ids corresponding to filter f
        vector<int> Pf;

        for (int i = 0; i < (int)points.size(); i++)
            if (points[i].category == f)
                Pf.push_back(i);

        // If there is no data for the filter, continue
        if (Pf.empty())
            continue;

        // Sample random points from Pf
        vector<int> Rf = sample(Pf, thres);

        // Select the first sample as the medoid
        int p_star = Rf[0];

        // Update the map M
        M[f] = p_star; // Use int for the filter
    }

    return M;
}
