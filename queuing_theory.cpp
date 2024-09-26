#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

const size_t ARRAY_SIZE = 1024 * 1024 * 128; // 128 MB
const size_t NUM_ITERATIONS = 1000; // Number of iterations for averaging

// Function to measure read and write latency and throughput
void measure_latency_throughput(double read_ratio, double write_ratio) {
    std::vector<int> arr(ARRAY_SIZE);
    volatile int tmp; // Use volatile to prevent optimization
    double total_read_time = 0;
    double total_write_time = 0;

    for (size_t iter = 0; iter < NUM_ITERATIONS; ++iter) {
        // Measure read latency
        auto read_start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < ARRAY_SIZE; ++i) {
            if ((i % (int)(1.0 / read_ratio)) == 0) {
                tmp = arr[i]; // Read operation
            }
        }
        auto read_end = std::chrono::high_resolution_clock::now();
        total_read_time += std::chrono::duration<double>(read_end - read_start).count();

        // Measure write latency
        auto write_start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < ARRAY_SIZE; ++i) {
            if ((i % (int)(1.0 / write_ratio)) == 0) {
                arr[i] = i; // Write operation
            }
        }
        auto write_end = std::chrono::high_resolution_clock::now();
        total_write_time += std::chrono::duration<double>(write_end - write_start).count();
    }

    // Calculate average latency in nanoseconds
    double avg_read_latency = (total_read_time / NUM_ITERATIONS) * 1e9 / (ARRAY_SIZE * read_ratio);
    double avg_write_latency = (total_write_time / NUM_ITERATIONS) * 1e9 / (ARRAY_SIZE * write_ratio);

    // Calculate throughput in MB/s
    double read_throughput = (ARRAY_SIZE * read_ratio * sizeof(int)) / (total_read_time / NUM_ITERATIONS) / (1024 * 1024);
    double write_throughput = (ARRAY_SIZE * write_ratio * sizeof(int)) / (total_write_time / NUM_ITERATIONS) / (1024 * 1024);

    std::cout << "Read Ratio: " << read_ratio << ", Write Ratio: " << write_ratio << std::endl;
    std::cout << "Average Read Latency: " << avg_read_latency << " ns, Read Throughput: " << read_throughput << " MB/s" << std::endl;
    std::cout << "Average Write Latency: " << avg_write_latency << " ns, Write Throughput: " << write_throughput << " MB/s" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

int main() {
    // Test different read/write ratios
    double ratios[][2] = {
        {1.0, 0.0}, // Read only
        {0.0, 1.0}, // Write only
        {0.7, 0.3}, // 70% Read, 30% Write
        {0.5, 0.5}  // 50% Read, 50% Write
    };

    for (auto& ratio : ratios) {
        measure_latency_throughput(ratio[0], ratio[1]);
    }

    return 0;
}
