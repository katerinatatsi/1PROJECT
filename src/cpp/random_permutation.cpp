#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

vector<int> generateRandomPermutation(int N) {
    // Create a vector with numbers 0 to N-1
    vector<int> numbers(N);
    for (int i = 0; i < N; ++i) {
        numbers[i] = i;
    }

    // Seed and create a random number generator
    random_device rd;  // Seed generator
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    // Shuffle the numbers to get a random permutation
    shuffle(numbers.begin(), numbers.end(), gen);

    return numbers;
}