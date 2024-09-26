#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <cstdlib>

const size_t CACHE_SIZE_L1 = 32 * 1024; // 32 KB L1 cache size (adjust based on your CPU)
const size_t CACHE_SIZE_L2 = 256 * 1024; // 256 KB L2 cache size (adjust based on your CPU)
const size_t ARRAY_SIZE =  1024 * 1024; // 1 million elements for the test

// Function to perform multiplication and measure time
double measure_performance(const std::vector<int>& data) {
    volatile int tmp; // Volatile to prevent optimization
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < data.size(); ++i) {
        tmp = data[i] * 2; // Light computation (multiplication)
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count(); // Return elapsed time in seconds
}

// Function to generate an access pattern that causes cache misses
void generate_cache_miss_pattern(std::vector<int>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        // Access elements in a way that will likely cause cache misses
        size_t index = (rand() % (data.size() / 2)) * 2; // Access every other element
        data[i] = index; // Fill the data to avoid uninitialized access
    }
}

int main() {
    // Initialize a vector of integers
    std::vector<int> data(ARRAY_SIZE);

    // Fill the vector with data to access
    generate_cache_miss_pattern(data);

    // Measure performance
    double elapsed_time = measure_performance(data);

    // Calculate operations performed
    double ops_performed = static_cast<double>(ARRAY_SIZE);

    std::cout << "Elapsed Time: " << elapsed_time << " seconds" << std::endl;
    std::cout << "Performance (ops/sec): " << ops_performed / elapsed_time << std::endl;

    return 0;
}
