#pragma once
#include <string>

namespace disk_benchmark {

void disk_io_benchmark(const std::string& filename, size_t file_size_mb, size_t buffer_size_kb);

void run();

} // namespace disk_benchmark
