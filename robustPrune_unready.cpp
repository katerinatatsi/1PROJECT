#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#define N 5 // Αριθμός κόμβων (μπορείς να αλλάξεις αυτόν τον αριθμό)
#define D 128 // Διαστάσεις

using namespace std;

//για medoid

struct Point {
    float coords[D];  // Συντεταγμένες στον 128-διάστατο χώρο
    bool operator==(const Point& other) const {
        return coords[0] == other.coords[0] && coords[1] == other.coords[1];
    }
};

//Παίρνω σαν παράμετρο ένα candidate set V που είναι επιλεγμένο σύνολο k κοντινότερων γειτόνων απ' την greedy
//Έξω ένα σύνολο Nout το οποίο το αρχικοποιώ κενό


// Συνάρτηση που υπολογίζει την Ευκλείδεια απόσταση μεταξύ δύο σημείων
double euclidean_distance(const Point& a, const Point& b) {
    double sum = 0.0;
    for (int i = 0; i < D; i++) {
        sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
    }
    return sqrt(sum);
}

vector<Point> robustPrune(const Point& p, const vector<Point>& candidates, double alpha, int r) {

    //Afto to candidates pou tha dinei stoixeia sto V me kapoia morfh tha to pairnw etoimo apo to greedy
    vector<pair<size_t, Point>> V;
    for (size_t i = 0; i < candidates.size(); ++i) {
        V.push_back({i, candidates[i]});
    }    

    //vector<Point> V = candidates;
    vector<Point> out_neighbors;
    out_neighbors.reserve(r);

    vector<bool> processed(candidates.size(), false);

    while(!V.empty()){
        
        vector<pair<size_t, Point>>::iterator p_star = 
            min_element(V.begin(), V.end(),
            [&p](const pair<size_t, Point>& a, const pair<size_t, Point>& b) {
                return euclidean_distance(p, a.second) < euclidean_distance(p, b.second);
            }
        );

        out_neighbors.push_back(p_star->second);
        processed[p_star->first] = true;


        if(out_neighbors.size() == r) break;

        V.erase(p_star);

        vector<pair<size_t, Point>> new_V;
        for(size_t i = 0; i < V.size(); ++i){
            size_t idx = V[i].first;
            Point p_prime = V[i].second;

            if(processed[idx]) continue;

            Point last_selected = out_neighbors.back();

            // h sinthiki tou pruning me to alpha, mia me 1 mia me >1 leei to paper
            float dist_through_last = alpha * euclidean_distance(last_selected, p_prime);
            float direct_dist = euclidean_distance(p, p_prime);

            if (dist_through_last > direct_dist) {
                new_V.push_back(make_pair(idx, p_prime));
            }
        }
        V = new_V;
    }
    return out_neighbors;
}