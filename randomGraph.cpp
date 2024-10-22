#include <iostream>
#include <iomanip>

#define N 5 // Αριθμός κόμβων (μπορείς να αλλάξεις αυτόν τον αριθμό)
#define R 4   // Κανονικότητα (in-degree και out-degree, μπορείς να το αλλάξεις)

using namespace std;

struct Point {
    float coords[28];  // Συντεταγμένες στον 28-διάστατο χώρο
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

    return 0;
}
