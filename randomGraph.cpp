#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <fstream>
#define N 5 // Αριθμός κόμβων (μπορείς να αλλάξεις αυτόν τον αριθμό)
#define R 4   // Κανονικότητα (in-degree και out-degree, μπορείς να το αλλάξεις)
#define D 128 // Διαστάσεις

using namespace std;

//για medoid

struct PointMedoid {
    float coords[D];  // Συντεταγμένες στον 128-διάστατο χώρο
};

// Συνάρτηση που υπολογίζει την Ευκλείδεια απόσταση μεταξύ δύο σημείων
double euclidean_distance(const PointMedoid& a, const PointMedoid& b) {
    double sum = 0.0;
    for (int i = 0; i < D; i++) {
        sum += (a.coords[i] - b.coords[i]) * (a.coords[i] - b.coords[i]);
    }
    return sqrt(sum);
}

// Συνάρτηση που βρίσκει το medoid με τη μέθοδο brute force
int find_medoid(const vector<PointMedoid>& points) {
    int n = points.size();
    double min_total_distance = numeric_limits<double>::infinity();
    int medoid_index = -1;

    for (int i = 0; i < n; i++) {
        double total_distance = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                total_distance += euclidean_distance(points[i], points[j]);
            }
        }
        if (total_distance < min_total_distance) {
            min_total_distance = total_distance;
            medoid_index = i;
        }
    }
    return medoid_index;
}


//τελος medoid

struct Point {
    float coords[128];  // Συντεταγμένες στον 28-διάστατο χώρο
};

// Συνάρτηση για την εμφάνιση του πίνακα γειτνίασης
void printAdjMatrix(int adjMatrix[N][N]) {
    cout << "Adjacency Matrix:\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Συνάρτηση για την εμφάνιση των γειτόνων κάθε κόμβου
void printNeighbors(int adjMatrix[N][N], int inDegree[N], int outDegree[N]) {
    cout << "\nNeighbors for each node:\n";
    for (int i = 0; i < N; i++) {
        cout << "Node " << i << ":\n";
        cout << "  Outgoing neighbors (out-degree = " << outDegree[i] << "): ";
        for (int j = 0; j < N; j++) {
            if (adjMatrix[i][j] == 1) {
                cout << j << " ";
            }
        }
        cout << endl;

        cout << "  Incoming neighbors (in-degree = " << inDegree[i] << "): ";
        for (int j = 0; j < N; j++) {
            if (adjMatrix[j][i] == 1) {
                cout << j << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    Point points[N];
    int adjMatrix[N][N] = {0};  // Πίνακας γειτνίασης
    int inDegree[N] = {0}, outDegree[N] = {0};  // In-degree και Out-degree

    // Δημιουργία R-regular γράφου με κυκλική διάταξη
    for (int i = 0; i < N; i++) {
        for (int k = 1; k <= R; k++) {
            // Ελέγχει αν ο κόμβος έχει λιγότερους από R γείτονες
            if (outDegree[i] < R) {
                // Προσθήκη ακμών
                int neighbor = (i + k) % N; // Υπολογισμός του γείτονα
                if (adjMatrix[i][neighbor] == 0) { // Αν δεν υπάρχει ήδη ακμή
                    adjMatrix[i][neighbor] = 1; // Προσθήκη ακμής
                    outDegree[i]++; // Ενημέρωση out-degree
                    inDegree[neighbor]++; // Ενημέρωση in-degree
                }
            }
        }
    }

    // Εμφάνιση πίνακα γειτνίασης
    printAdjMatrix(adjMatrix);

    // Εμφάνιση γειτόνων για κάθε κόμβο
    printNeighbors(adjMatrix, inDegree, outDegree);

    // Έλεγχος αν είναι r-regular
    bool isRRegular = true;
    for (int i = 0; i < N; i++) {
        if (inDegree[i] != R || outDegree[i] != R) {
            isRRegular = false;
            break;
        }
    }

    if (isRRegular) {
        cout << "\nThe graph is " << R << "-regular.\n";
    } else {
        cout << "\nThe graph is NOT " << R << "-regular.\n";
    }

    //medoid

    vector<PointMedoid> pointVec(N);
    ifstream file("points.txt");
    if (!file) {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    // Διαβάζουμε τα σημεία από το αρχείο
    for (int i = 0; i < N; i++) {
        cout << "Reading point " << i << ": ";
        for (int j = 0; j < D; j++) {
            if (!(file >> pointVec[i].coords[j])) {
                cerr << "Error reading coordinates at point " << i << ", coordinate " << j << endl;
                return 1;
            }
            cout << pointVec[i].coords[j] << " ";
        }
        cout << endl;
    }
    file.close();

    // Βρίσκουμε το medoid
    int medoid_index = find_medoid(pointVec);

    // Εκτύπωση του Medoid
    cout << "\nMedoid Index: " << medoid_index << endl;
    cout << "Medoid Coordinates: ";
    for (int j = 0; j < D; j++) {
        cout << fixed << setprecision(2) << pointVec[medoid_index].coords[j] << " ";
    }
    cout << endl;

    //τελος medoid

    return 0;
}