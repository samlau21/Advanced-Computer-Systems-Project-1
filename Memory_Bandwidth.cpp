#include <iostream>
#include <chrono>
#include <vector>

const int ARRAY_SIZE = 1024 * 1024 * 128; // 128 MB array
const int CACHE_LINE_SIZE = 64; // Size in bytes (you can adjust this)

// Function to perform memory read/write operations
void memory_bandwidth_test(size_t granularity, double read_ratio, double write_ratio) {
    // Create a vector to hold the data
    std::vector<int> arr(ARRAY_SIZE);
    volatile int tmp; // Use volatile to prevent optimization

    // Determine the number of iterations based on the granularity
    size_t iterations = ARRAY_SIZE * sizeof(int) / granularity;
    
    auto start = std::chrono::high_resolution_clock::now();

    // Execute read and write operations based on the specified ratios
    for (size_t i = 0; i < iterations; ++i) {
        size_t index = (i * granularity / sizeof(int)) % ARRAY_SIZE; // wrap around to fit in array

        if (read_ratio > 0 && (i % (int)(1.0 / read_ratio)) == 0) {
            tmp = arr[index]; // Read operation
        }
        if (write_ratio > 0 && (i % (int)(1.0 / write_ratio)) == 0) {
            arr[index] = index; // Write operation
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Calculate total bytes processed
    double total_bytes = (read_ratio + write_ratio) * iterations * granularity;
    double bandwidth = total_bytes / (elapsed.count() * (1024 * 1024)); // in MB/s

    std::cout << "Granularity: " << granularity << " B, "
              << "Read Ratio: " << read_ratio << ", "
              << "Write Ratio: " << write_ratio << ", "
              << "Bandwidth: " << bandwidth << " MB/s" << std::endl;
}

int main() {
    // Define different granularities and read/write ratios
    size_t granularities[] = {64, 256, 1024}; // in bytes
    double ratios[][2] = {
        {1.0, 0.0}, // Read only
        {0.0, 1.0}, // Write only
        {0.7, 0.3}, // 70% Read, 30% Write
        {0.5, 0.5}  // 50% Read, 50% Write
    };

    // Test bandwidth for different granularities and ratios
    for (size_t granularity : granularities) {
        for (auto& ratio : ratios) {
            memory_bandwidth_test(granularity, ratio[0], ratio[1]);
        }
    }

    return 0;
}
