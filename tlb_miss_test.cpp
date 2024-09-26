#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>

const size_t ARRAY_SIZE = 1024 * 1024 * 128; // 128 MB array
const size_t NUM_OPERATIONS = 1000000; // Number of operations to perform

// Function to perform multiplication and measure time
double measure_performance(const std::vector<int>& data) {
    volatile int tmp; // Volatile to prevent optimization
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < NUM_OPERATIONS; ++i) {
        size_t index = rand() % data.size(); // Random access to generate TLB misses
        tmp = data[index] * 2; // Light computation (multiplication)
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count(); // Return elapsed time in seconds
}

int main() {
    // Initialize a vector of integers
    std::vector<int> data(ARRAY_SIZE);

    // Fill the vector with data
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = i; // Populate data
    }

    // Measure performance
    double elapsed_time = measure_performance(data);

    // Calculate operations performed
    double ops_performed = static_cast<double>(NUM_OPERATIONS);

    std::cout << "Elapsed Time: " << elapsed_time << " seconds" << std::endl;
    std::cout << "Performance (ops/sec): " << ops_performed / elapsed_time << std::endl;

    return 0;
}
