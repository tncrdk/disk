#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <random> // For generating random data


namespace disk_benchmark {

void disk_io_benchmark(const std::string& filename, size_t file_size_mb, size_t buffer_size_kb) {

    size_t file_size_bytes = file_size_mb * 1024 * 1024;
    size_t buffer_size_bytes = buffer_size_kb * 1024;
    std::vector<char> buffer(buffer_size_bytes);

    // Fill buffer with random data for more realistic writes
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);
    for (size_t i = 0; i < buffer_size_bytes; ++i) {
        buffer[i] = static_cast<char>(distrib(gen));
    }

    // --- Write Benchmark ---
    std::cout << "------------------------" << std::endl;
    std::cout << "Benchmarking Disk Write Performance (" << file_size_mb << " MB, buffer " << buffer_size_kb << " KB)..." << std::endl;
    auto start_write = std::chrono::high_resolution_clock::now();
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc); // truncates existing file
    if (!ofs.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (size_t i = 0; i < file_size_bytes; i += buffer_size_bytes) {
        ofs.write(buffer.data(), buffer_size_bytes);
    }
    ofs.close();
    auto end_write = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_write = end_write - start_write;
    double write_speed_mbps = static_cast<double>(file_size_bytes) / (1024.0 * 1024.0) / duration_write.count();
    std::cout << "Write time: " << duration_write.count() << " seconds, Speed: " << write_speed_mbps << " MB/s" << std::endl;

    // --- Read Benchmark ---
    std::cout << "\nBenchmarking Disk Read Performance (" << file_size_mb << " MB, buffer " << buffer_size_kb << " KB)..." << std::endl;
    auto start_read = std::chrono::high_resolution_clock::now();
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return;
    }

    long long read_sum_val = 0; // To prevent read optimization
    while (ifs.read(buffer.data(), buffer_size_bytes)) {
        for (char c : buffer) {
            read_sum_val += c;
        }
    }
    ifs.close();
    auto end_read = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_read = end_read - start_read;
    double read_speed_mbps = static_cast<double>(file_size_bytes) / (1024.0 * 1024.0) / duration_read.count();
    std::cout << "Read time: " << duration_read.count() << " seconds, Speed: " << read_speed_mbps << " MB/s" << std::endl;

    std::cout << "------------------------\n" << std::endl;

    // Clean up the created file
    std::remove(filename.c_str());
    
    // To ensure read_sum_val is not optimized away
    volatile long long temp_sum_val = read_sum_val;
}

void run() {
    std::string test_filename = "benchmark_test_file.bin";
    size_t test_file_size_mb = 1024; // 1 GB
    size_t test_buffer_size_kb = 4; // Typical disk block size

    disk_io_benchmark(test_filename, test_file_size_mb, test_buffer_size_kb);
}

} // namespace disk_benchmark
