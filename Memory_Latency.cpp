#include <iostream>
#include <chrono>
#include <vector>

const size_t CACHE_SIZE_L1 = 32 * 1024; // 32 KB L1 cache size (adjust based on your CPU)
const size_t CACHE_SIZE_L2 = 256 * 1024; // 256 KB L2 cache size (adjust based on your CPU)
const size_t CACHE_SIZE_L3 = 4 * 1024 * 1024; // 4 MB L3 cache size (adjust based on your CPU)
const size_t ARRAY_SIZE = CACHE_SIZE_L1 / sizeof(int); // Size of array to fit L1 cache

// Function to measure read latency
void measure_read_latency(int* arr) {
    volatile int tmp; // Volatile to prevent compiler optimizations
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        tmp = arr[i]; // Read operation
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    double latency = elapsed.count() / ARRAY_SIZE * 1e9; // Convert to nanoseconds
    std::cout << "Read Latency: " << latency << " ns" << std::endl;
}

// Function to measure write latency
void measure_write_latency(int* arr) {
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = i; // Write operation
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    double latency = elapsed.count() / ARRAY_SIZE * 1e9; // Convert to nanoseconds
    std::cout << "Write Latency: " << latency << " ns" << std::endl;
}

int main() {
    // Allocate memory buffer
    std::vector<int> arr(ARRAY_SIZE);

    // Measure read and write latencies
    measure_read_latency(arr.data()); // Read latency
    measure_write_latency(arr.data()); // Write latency

    return 0;
}
