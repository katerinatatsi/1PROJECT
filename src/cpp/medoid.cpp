#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <fstream>

#include "../hpp/medoid.hpp"
#include "../hpp/Point.hpp"

using namespace std;

void printMedoid(const vector<Point>& points, int medoidIndex) {
    Point medoidPoint = points[medoidIndex];

    cout << "Medoid Index: " << medoidIndex << endl;
    cout << "Medoid Coordinates: ";

    for (float v: medoidPoint.coords)
        cout << fixed << setprecision(2) << v << " ";
    
    cout << endl;
}

int findMedoidIndex(const vector<Point>& points) {
    double minTotalDistance = numeric_limits<double>::infinity();
    int medoidIndex = -1;

    for (int i = 0; i < (int)points.size(); i++) {
        double totalDistance = 0.0;
        for (int j = 0; j < (int)points.size(); j++)
            if (i != j) 
                totalDistance += points[i] - points[j];
            
        if (totalDistance < minTotalDistance) {
            minTotalDistance = totalDistance;
            medoidIndex = i;
        }
    }

    return medoidIndex;
}